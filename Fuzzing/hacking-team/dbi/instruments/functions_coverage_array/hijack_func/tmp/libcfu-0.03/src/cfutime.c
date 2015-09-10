/* Creation date: 2005-06-26 18:11:40
 * Authors: Don
 * Change log:
 */

/* Copyright (c) 2005 Don Owens
   All rights reserved.

   This code is released under the BSD license:

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

     * Redistributions in binary form must reproduce the above
       copyright notice, this list of conditions and the following
       disclaimer in the documentation and/or other materials provided
       with the distribution.

     * Neither the name of the author nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
   COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
   OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cfu.h"
#include "cfutime.h"
#include <string.h>
#include <stdlib.h>

#ifdef CFU_DEBUG
#ifdef NDEBUG
#undef NDEBUG
#endif
#else
#ifndef NDEBUG
#define NDEBUG 1
#endif
#endif
#include <assert.h>

typedef struct cfutime {
	libcfu_type type;
	struct timeval begin_tv;
	struct timezone begin_tz;
	struct timeval end_tv;
	struct timezone end_tz;
} cfutime;

extern cfutime_t *
cfutime_new() {
	cfutime_t *time = (cfutime_t *)calloc(1, sizeof(cfutime));
	time->type = libcfu_t_time;
	return time;
}

extern void
cfutime_begin(cfutime_t *time) {
	gettimeofday(&(time->begin_tv), &(time->begin_tz));
}

extern void
cfutime_end(cfutime_t *time) {
	gettimeofday(&(time->end_tv), &(time->end_tz));
}

extern double
cfutime_elapsed(cfutime_t *time) {
	return (double)time->end_tv.tv_sec + ((double)time->end_tv.tv_usec)/1000000 -
		((double)time->begin_tv.tv_sec + ((double)time->begin_tv.tv_usec)/1000000);
}

extern void
cfutime_free(cfutime_t *time) {
	free(time);
}
