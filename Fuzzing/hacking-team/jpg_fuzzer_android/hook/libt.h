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

#define log_start(...) \
        {FILE *f = fopen("/data/local/tmp/log_start", "a+");\
        fprintf(f, __VA_ARGS__);\
        fflush(f); fclose(f); }


#define DEBUG 0
#define DEBUG_POST 0
#define LOCK_DEBUG 0

struct hook_t {
  unsigned int jump[4];		// ARM jump code
  unsigned int store[4];	// ARM stolen bytes

  unsigned char jumpt[22];	// Thumb jump code    (was 12)
  unsigned char storet[22];	// Thumb stolen bytes (was 12)

  unsigned int orig;		// Hooked function address
  unsigned int patch;		

  unsigned char thumb;		// ARM/thumb switch     
  unsigned char name[128];	// function name

  unsigned int  lib_base_addr;  // base address for the library
  char libname[32];             // name of library for log

  UT_hash_handle hh;

  void *data;
};

char filename[128];



void hook_precall(struct hook_t *h);
void hook_postcall(struct hook_t *h);
int hook(struct hook_t *h, int pid, char *libname, char *funcname, void *hookf, int by_address, unsigned int raw_address);
void unhook(struct hook_t *h);
