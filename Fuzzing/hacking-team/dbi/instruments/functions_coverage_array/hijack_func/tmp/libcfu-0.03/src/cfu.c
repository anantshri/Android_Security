/* Creation date: 2005-07-09 21:44:59
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

struct libcfu_item {
	libcfu_type type;
};

extern libcfu_type
cfu_get_type(void *item) {
	if (!item) return libcfu_t_none;
	return ((libcfu_item_t *)item)->type;
}

extern int
cfu_is_hash(void *item) {
	if (cfu_get_type(item) == libcfu_t_hash_table) return 1;
	return 0;
}

extern int
cfu_is_list(void *item) {
	if (cfu_get_type(item) == libcfu_t_list) return 1;
	return 0;
}

extern int
cfu_is_string(void *item) {
	if (cfu_get_type(item) == libcfu_t_string) return 1;
	return 0;
}

extern int
cfu_is_time(void *item) {
	if (cfu_get_type(item) == libcfu_t_time) return 1;
	return 0;
}

extern int
cfu_is_conf(void *item) {
	if (cfu_get_type(item) == libcfu_t_conf) return 1;
	return 0;
}
