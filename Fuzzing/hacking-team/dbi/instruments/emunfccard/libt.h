/*
 *  Collin's Binary Instrumentation Tool/Framework for Android
 *  Collin Mulliner <collin[at]mulliner.org>
 *  http://www.mulliner.org/android/
 *
 *  (c) 2012
 *
 *  License: GPL v2
 *
 */

#include "phDal4Nfc_messageQueueLib.h"

#define log(...) \
        {FILE *f = fopen("/data/local/tmp/log", "a+");\
        fprintf(f, __VA_ARGS__);\
        fflush(f); fclose(f); }

struct hook_t {
	unsigned int jump[3];
	unsigned int store[3];
	unsigned char jumpt[12];
	unsigned char storet[12];
	unsigned int orig;
	unsigned int patch;
	unsigned char thumb;
	unsigned char name[128];
	void *data;
};

void hook_precall(struct hook_t *h);
void hook_postcall(struct hook_t *h);
 int hook(struct hook_t *h, int pid, char *libname, char *funcname, void *hookf);
void unhook(struct hook_t *h);

int fake_card_state;
int status_wait;

phDal4Nfc_Message_Wrapper_t wrapper;
int global_msqid;
int msgsend;
