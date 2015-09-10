/*
 *  Collin's Binary Instrumentation Tool/Framework for Android
 *  Collin Mulliner <collin[at]mulliner.org>
 *
 *  (c) 2012
 *
 *  License: GPL v2
 *
 */
#include "uthash.h"


#define log(...) \
        {FILE *f = fopen("/data/local/tmp/log", "a+");\
        fprintf(f, __VA_ARGS__);\
        fflush(f); fclose(f); }

struct hook_t {
  unsigned int jump[3];		// ARM jump code
  unsigned int store[3];	// ARM stolen bytes

  unsigned char jumpt[28];	// Thumb jump code    (was 12)
  unsigned char storet[28];	// Thumb stolen bytes (was 12)

  unsigned int orig;		// Hooked function address
  unsigned int patch;		

  unsigned char thumb;		// ARM/thumb switch     
  unsigned char name[128];	// function name

  UT_hash_handle hh;

  void *data;
};

void hook_precall(struct hook_t *h);
void hook_postcall(struct hook_t *h);
int hook(struct hook_t *h, int pid, char *libname, char *funcname, void *hookf, int by_address, unsigned int raw_address);
void unhook(struct hook_t *h);
