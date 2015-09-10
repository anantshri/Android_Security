#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include "libt.h"
#include "util.h"

#include "hooker.h"
#include "uthash.h"


static int ip_register_offset = 0xf;

void* (*read_jpeg_ptr)(void* a, void* b);

void*  read_jpeg(void* a, void* b)  {
  
  void *result;
  int ip_var, r0, count;
  char path[128];
  int locked_here = 0;
  struct hook_t *hook_no_proto, *hooked_tmp, *hoo = NULL;

  asm("mov %[ip], ip" : [ip] "=r" (ip_var) ); // original address in r12 - 0xf
  ip_var -= ip_register_offset;

  asm("mov %[r0], r0" : [r0] "=r" (r0) ); // path address
  
  // Mutex for atomic operations on hash table
  if(tid_locker != gettid()) {      // Check if this thread locked already the mutex

    if(LOCK_DEBUG)
      log("Getting LOCK by %d\n",gettid())

    pthread_mutex_lock(&M);         // If not lock the mutex

    if(LOCK_DEBUG)
      log("LOCK got by %d\n",gettid())

    locked_here = 1;
    tid_locker = gettid();
  }


  //find struct hook_t 
  HASH_FIND_INT(hook_hash1, &ip_var, hook_no_proto);
  read_jpeg_ptr = (void *) hook_no_proto->orig;
  hook_precall(hook_no_proto);

  result = read_jpeg_ptr(a, b);

  log("%x thumb %x\n", ip_var - hook_no_proto->lib_base_addr, hook_no_proto->lib_base_addr)

  if(strcmp(filename, (void *)r0)) {
    log("file:%s \n", r0)
    count = HASH_COUNT(hooked_list_hash);
    strncpy(filename, (void *)r0, sizeof(filename));
    if(DEBUG_POST)
      log("hooked %d func\n", count);
    // When a new file i detected re-install previously executed hook and clear the hash-table
    HASH_ITER(hh, hooked_list_hash, hoo, hooked_tmp) {
      hook_postcall(hoo);
    }
    HASH_CLEAR(hh,hooked_list_hash);
  }

  // Install again the hook for readjpeg
  hook_postcall(hook_no_proto);
 
  if(tid_locker == gettid() && locked_here) {
    locked_here = 0;
    tid_locker = 0;
    pthread_mutex_unlock(&M);

    if(LOCK_DEBUG)
      log("LOCK released by %d\n",gettid())
  }

  return result;

}
