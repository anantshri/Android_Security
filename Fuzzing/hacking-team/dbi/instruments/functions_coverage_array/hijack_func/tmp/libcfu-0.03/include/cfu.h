/* Creation date: 2005-07-06 07:34:51
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

#ifndef _LIBCFU_H_
#define _LIBCFU_H_

#include <stdio.h>
#include <cfutypes.h>

#ifdef __cplusplus
#define CFU_BEGIN_DECLS extern "C" {
#define CFU_END_DECLS }
#else
#define CFU_BEGIN_DECLS
#define CFU_END_DECLS
#endif

CFU_BEGIN_DECLS

/* define this to get thread-safe versions of functions */
#ifndef _REENTRANT
#define _REENTRANT 1
#endif
#ifndef _THREAD_SAFE
#define _THREAD_SAFE 1
#endif

#define LIBCFU_VERSION "0.03"

typedef enum { libcfu_t_none = 0, libcfu_t_hash_table, libcfu_t_list, libcfu_t_string,
			   libcfu_t_time, libcfu_t_conf } libcfu_type;

struct libcfu_item;
typedef struct libcfu_item libcfu_item_t;

extern libcfu_type cfu_get_type(void *item);
extern int cfu_is_hash(void *item);
extern int cfu_is_list(void *item);
extern int cfu_is_string(void *item);
extern int cfu_is_time(void *item);
extern int cfu_is_conf(void *item);

CFU_END_DECLS

#endif
