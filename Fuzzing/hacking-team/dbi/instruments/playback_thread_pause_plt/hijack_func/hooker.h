#include "../uthash.h"

struct hook_t *hook_hash1;
struct hook_t *postcall_hash;

#define ROUNDUP(x, y) ((((x)+((y)-1))/(y))*(y))


struct cblk_t {

  unsigned long playbackLastBufferStartAddress_array;
  unsigned long streamType;

  UT_hash_handle hh;

};


unsigned int base_address;

void* no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

// 1] function prototype
void* pause_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* setStreamVolume_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* newTrack_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* startTrack_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* stopTrack_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* threadLoop_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* audioFlingerC1_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* audioFlingerC2_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* getBuffer_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* stepUser_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* stepServer_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* recordTrack_getNextBuffer_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* playbackTrack_getNextBuffer_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* recordThread_getNextBuffer_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;
void* playbackTimedTrack_getNextBuffer_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* newRecordTrack_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

/* signaling */
void* recordTrackStart_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* playbackTrackStart_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* recordTrackStop_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

void* playbackTrackStop_h(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;

// 2] hook
struct hook_t pause_plt_hook;
struct hook_t setStreamVolume_hook;

struct hook_t newTrack_hook;
struct hook_t startTrack_hook;
struct hook_t stopTrack_hook;
struct hook_t threadLoop_hook;

struct hook_t audioFlingerC1_hook;
struct hook_t audioFlingerC2_hook;

struct hook_t getBuffer_hook;

struct hook_t stepUser_hook;
struct hook_t stepServer_hook;

struct hook_t recordTrack_getNextBuffer_hook;
struct hook_t playbackTrack_getNextBuffer_hook;

struct hook_t recordThread_getNextBuffer_hook;
struct hook_t playbackTimedTrack_getNextBuffer_hook;

/* signaling */
struct hook_t playbackTrackStart_hook;
struct hook_t recordTrackStart_hook;
struct hook_t playbackTrackStop_hook;
struct hook_t recordTrackStop_hook;


struct hook_t newRecordTrack_hook;


unsigned long audioFlingerInstance;
unsigned long pbInstance;

// getBuffer globals
unsigned long bufferStart;
unsigned long lastSize;
unsigned long lastOffset;

unsigned long lastFrameSize;
unsigned long lastBufferStart;
unsigned long lastBufferEnd;

// recordTrack_getNextBuffer globals
unsigned long lastBufferStartAddress;

// playbackTrack_getNextBuffer globals
//unsigned long playbackLastBufferStartAddress;

// 3] insert the hook
#define HOOK_coverage_0 hook_no_hash(&pause_plt_hook, pid, "libaudioflinger", "", pause_h, 0,  0x42a3f);
#define HOOK_coverage_1 hook_no_hash(&setStreamVolume_hook, pid, "libaudioflinger", "", setStreamVolume_h, 0,  0x427f9);



#define HOOK_coverage_3 hook_no_hash(&startTrack_hook, pid, "libaudioflinger", "_ZN7android18AudioPolicyService11startOutputEi19audio_stream_type_ti", startTrack_h, 1,  0);
#define HOOK_coverage_4 hook_no_hash(&stopTrack_hook, pid, "libaudioflinger", "_ZN7android18AudioPolicyService10stopOutputEi19audio_stream_type_ti", stopTrack_h, 1,  0);



#define HOOK_coverage_5 hook_no_hash(&threadLoop_hook, pid, "libaudioflinger", "", threadLoop_h, 0,  0x33609);

#define HOOK_coverage_6 hook_no_hash(&audioFlingerC1_hook, pid, "libaudioflinger", "", audioFlingerC1_h, 0,  0x3fdad);
#define HOOK_coverage_7 hook_no_hash(&audioFlingerC2_hook, pid, "libaudioflinger", "", audioFlingerC2_h, 0,  0x3c6f5);

#define HOOK_coverage_8 hook_no_hash(&getBuffer_hook, pid, "libaudioflinger", "", getBuffer_h, 0,  0x35263);

// libmedia
#define HOOK_coverage_9 hook_no_hash(&stepUser_hook, pid, "libmedia", "", stepUser_h, 0,  0x44a89);
#define HOOK_coverage_10 hook_no_hash(&stepServer_hook, pid, "libmedia", "", stepServer_h, 0,  0x44c79);


#define HOOK_coverage_11 hook_no_hash(&recordTrack_getNextBuffer_hook, pid, "libaudioflinger", "_ZN7android12AudioFlinger12RecordThread11RecordTrack13getNextBufferEPNS_19AudioBufferProvider6BufferEx", recordTrack_getNextBuffer_h, 1,  0);//0x35275);
#define HOOK_coverage_12 hook_no_hash(&playbackTrack_getNextBuffer_hook, pid, "libaudioflinger", "_ZN7android12AudioFlinger14PlaybackThread5Track13getNextBufferEPNS_19AudioBufferProvider6BufferEx", playbackTrack_getNextBuffer_h, 1, 0);// 0x352d1);

#define HOOK_coverage_13 hook_no_hash(&recordThread_getNextBuffer_hook, pid, "libaudioflinger", "", recordThread_getNextBuffer_h, 0,  0x3345d);
#define HOOK_coverage_14 hook_no_hash(&playbackTimedTrack_getNextBuffer_hook, pid, "libaudioflinger", "", playbackTimedTrack_getNextBuffer_h, 0,  0x35931);

//#define HOOK_coverage_15 hook_no_hash(&newRecordTrack_hook, pid, "libaudioflinger", "_ZN7android12AudioFlinger12RecordThread11RecordTrackC2EPS1_RKNS_2spINS0_6ClientEEEj14audio_format_tjii", newRecordTrack_h, 1, 0);
//#define HOOK_coverage_15 hook_no_hash(&newRecordTrack_hook, pid, "libaudioflinger", "_ZN7android12AudioFlinger12RecordThread11RecordTrack5startENS_11AudioSystem12sync_event_tEi", newRecordTrack_h, 1, 0);


/* signaling hooks */

// PlaybackThead::Track
#define HOOK_coverage_2 hook_no_hash(&newTrack_hook, pid, "libaudioflinger", "_ZN7android12AudioFlinger14PlaybackThread5TrackC2EPS1_RKNS_2spINS0_6ClientEEE19audio_stream_type_tj14audio_format_tjiRKNS4_INS_7IMemoryEEEij", newTrack_h, 1,  0);
#define HOOK_coverage_17 hook_no_hash(&playbackTrackStart_hook, pid, "libaudioflinger", "_ZN7android12AudioFlinger14PlaybackThread5Track5startENS_11AudioSystem12sync_event_tEi", playbackTrackStart_h, 1, 0);
#define HOOK_coverage_19 hook_no_hash(&playbackTrackStop_hook, pid, "libaudioflinger", "_ZN7android12AudioFlinger14PlaybackThread5Track4stopEv", playbackTrackStop_h, 1, 0);


// RecordThread::RecordTrack
#define HOOK_coverage_16 hook_no_hash(&recordTrackStart_hook, pid, "libaudioflinger", "_ZN7android12AudioFlinger12RecordThread11RecordTrack5startENS_11AudioSystem12sync_event_tEi", recordTrackStart_h, 1, 0);
#define HOOK_coverage_18 hook_no_hash(&recordTrackStop_hook, pid, "libaudioflinger", "_ZN7android12AudioFlinger12RecordThread11RecordTrack4stopEv", recordTrackStop_h, 1, 0);
