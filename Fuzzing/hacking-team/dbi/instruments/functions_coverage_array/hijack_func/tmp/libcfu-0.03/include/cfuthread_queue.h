/* Creation date: 2005-07-26 08:19:33
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

#ifndef _CFUTHREAD_QUEUE_H
#define _CFUTHREAD_QUEUE_H

#include <cfu.h>

#ifdef __cplusplus
extern "C" {
#endif

	/* cfuthread_queue provides a way to serialize requests for a
	 * resource where you want the resource to be accessed from a
	 * single thread only.  For instance, for a database connection
	 * where making calls in separate threads does not work properly,
	 * you can use cfuthread_queue.  cfuthread_queue_new() creates a
	 * new thread that waits for something to be added to the queue.
	 * Once something is added, the thread will process the data by
	 * calling the function you pass as an argument to the
	 * cfuthread_queue_new() function.
	 */

	struct cfuthread_queue;
	typedef struct cfuthread_queue cfuthread_queue_t;

	typedef void * (*cfuthread_queue_fn_t)(void *arg);
	typedef void (*cfuthread_queue_init_t)(void *arg);
	typedef void (*cfuthread_queue_cleanup_t)(void *arg);

	/* Creates a new thread queue structure that will run the given
	 * function when a request is received.
	*/
	extern cfuthread_queue_t * cfuthread_queue_new(cfuthread_queue_fn_t fn);

	/* Same as cfuthread_queue_new(), but with an initialization
	 * function that gets called with the argument init_arg when the
	 * thread is created, and a cleanup function that gets called with
	 * the argument cleanup_arg when the thread exits, e.g., when
	 * cfuthread_queue_destroy() is called.
	 */
	extern cfuthread_queue_t * cfuthread_queue_new_with_cleanup(cfuthread_queue_fn_t fn,
		cfuthread_queue_init_t init_fn, void *init_arg, cfuthread_queue_cleanup_t cleanup_fn,
		void *cleanup_arg);

	/* Add a request to the queue.  data will get passed to the
	 * function fn given to cfuthread_queue_new when it reaches the
	 * front of the queue.
	 */
	extern void * cfuthread_queue_make_request(cfuthread_queue_t * tq, void *data);

	/* Free up resources used by the queue, in addition to canceling
	 * the thread.
	 */
	extern void cfuthread_queue_destroy(cfuthread_queue_t *);

#ifdef __cplusplus
}
#endif


#endif
