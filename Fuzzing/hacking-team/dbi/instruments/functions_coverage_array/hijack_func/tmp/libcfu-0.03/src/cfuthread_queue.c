/* Creation date: 2005-07-26 08:20:59

 * Authors: Don
 * Change log:

 */

#include "cfuthread_queue.h"
#include "cfulist.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

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

struct cfuthread_queue {
	pthread_mutex_t mutex;
	pthread_cond_t cv;
	cfulist_t *request_queue;
	cfuthread_queue_fn_t fn;
	pthread_t thread;
	cfuthread_queue_init_t init_fn;
	void *init_arg;
	cfuthread_queue_cleanup_t cleanup_fn;
	void *cleanup_arg;
};

typedef struct cfuthread_queue_entry {
	pthread_mutex_t mutex;
	pthread_cond_t cv;
	void *data_in;
	void *data_out;
} cfuthread_queue_entry;

static cfuthread_queue_entry *
_new_cfuthread_entry(void *data) {
	cfuthread_queue_entry *entry =
		(cfuthread_queue_entry *)calloc(1, sizeof(cfuthread_queue_entry));
	pthread_mutex_init(&entry->mutex, NULL);
	pthread_cond_init(&entry->cv, NULL);
	entry->data_in = data;
	return entry;
}

static void
_destroy_cfuthread_entry(cfuthread_queue_entry *entry) {
	pthread_mutex_destroy(&entry->mutex);
	pthread_cond_destroy(&entry->cv);
	free(entry);
}

static void *
_run_queue(void *arg) {
	cfuthread_queue_t *tq = (cfuthread_queue_t *)arg;
	cfuthread_queue_entry *request = NULL;

	if (tq->init_fn) {
		tq->init_fn(tq->init_arg);
	}

	pthread_cleanup_push((void *)tq->cleanup_fn, tq->cleanup_arg);
	
	while (1) {
		pthread_mutex_lock(&tq->mutex);
		while (cfulist_num_entries(tq->request_queue) == 0) {
			pthread_cond_wait(&tq->cv, &tq->mutex);
		}
		
		request = (cfuthread_queue_entry *)cfulist_dequeue(tq->request_queue);
		pthread_mutex_unlock(&tq->mutex);
		if (!request) continue;

		pthread_mutex_lock(&request->mutex);
		request->data_out = tq->fn(request->data_in);
		pthread_cond_signal(&request->cv);
		pthread_mutex_unlock(&request->mutex);
	}
	pthread_exit((void *)0);

	pthread_cleanup_pop(0);

}

extern cfuthread_queue_t *
cfuthread_queue_new_with_cleanup(cfuthread_queue_fn_t fn, cfuthread_queue_init_t init_fn,
	void *init_arg, cfuthread_queue_cleanup_t cleanup_fn,
	void *cleanup_arg) {
	cfuthread_queue_t *tq = (cfuthread_queue_t *)calloc(1, sizeof(cfuthread_queue_t));
	pthread_mutex_init(&tq->mutex, NULL);
	pthread_cond_init(&tq->cv, NULL);
	tq->fn = fn;
	tq->request_queue = cfulist_new();
	tq->init_fn = init_fn;
	tq->init_arg = init_arg;
	tq->cleanup_fn = cleanup_fn;
	tq->cleanup_arg = cleanup_arg;

	/* FIXME: do pthread_create() here to run a function that waits on
	   an entry to be put into the queue, then call fn().
	*/
	if ( (0 != pthread_create(&tq->thread, NULL, _run_queue, (void *)tq)) ) {
		return NULL;
	}
	
	return tq;
}

extern cfuthread_queue_t *
cfuthread_queue_new(cfuthread_queue_fn_t fn) {
	return cfuthread_queue_new_with_cleanup(fn, NULL, NULL, NULL, NULL);
}

extern void *
cfuthread_queue_make_request(cfuthread_queue_t * tq, void *data) {
	cfuthread_queue_entry *request = _new_cfuthread_entry(data);

	pthread_mutex_lock(&tq->mutex);
	pthread_mutex_lock(&request->mutex);
	cfulist_enqueue(tq->request_queue, (void *)request);
	pthread_cond_signal(&tq->cv);
	pthread_mutex_unlock(&tq->mutex);

	pthread_cond_wait(&request->cv, &request->mutex);
	pthread_mutex_unlock(&request->mutex);

	data = request->data_out;
	_destroy_cfuthread_entry(request);

	return data;
}

extern void
cfuthread_queue_destroy(cfuthread_queue_t *tq) {
	void *rv = NULL;

	pthread_cancel(tq->thread);
	pthread_join(tq->thread, &rv);
	pthread_mutex_destroy(&tq->mutex);
	pthread_cond_destroy(&tq->cv);
	cfulist_destroy(tq->request_queue);
	free(tq);
}
