/* Creation date: 2005-06-24 21:22:40
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

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "cfuhash.h"
#include "cfustring.h"

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

typedef struct cfuhash_event_flags {
	int resized:1;
	int pad:31;
} cfuhash_event_flags;

typedef struct cfuhash_entry {
	void *key;
	size_t key_size;
	void *data;
	size_t data_size;
	struct cfuhash_entry *next;
} cfuhash_entry;

struct cfuhash_table {
	libcfu_type type;
	size_t num_buckets;
	size_t entries; /* Total number of entries in the table. */
	cfuhash_entry **buckets;
	pthread_mutex_t mutex;
	u_int32_t flags;
	cfuhash_function_t hash_func;
	size_t each_bucket_index;
	cfuhash_entry *each_chain_entry;
	float high;
	float low;
	cfuhash_free_fn_t free_fn;
	unsigned int resized_count;
	cfuhash_event_flags event_flags;
};

/* Perl's hash function */
static u_int32_t
hash_func(const void *key, size_t length) {
	register size_t i = length;
	register u_int hv = 0; /* could put a seed here instead of zero */
	register const unsigned char *s = (char *)key;
	while (i--) {
		hv += *s++;
		hv += (hv << 10);
		hv ^= (hv >> 6);
	}
	hv += (hv << 3);
	hv ^= (hv >> 11);
	hv += (hv << 15);

	return hv;
}

/* makes sure the real size of the buckets array is a power of 2 */
static u_int
hash_size(u_int s) {
	u_int i = 1;
	while (i < s) i <<= 1;
	return i;
}

static inline void *
hash_key_dup(const void *key, size_t key_size) {
	void *new_key = malloc(key_size);
	memcpy(new_key, key, key_size);
	return new_key;
}

static inline void *
hash_key_dup_lower_case(const void *key, size_t key_size) {
	char *new_key = (char *)hash_key_dup(key, key_size);
	size_t i = 0;
	for (i = 0; i < key_size; i++) new_key[i] = tolower(new_key[i]);
	return (void *)new_key;
}

/* returns the index into the buckets array */
static inline u_int
hash_value(cfuhash_table_t *ht, const void *key, size_t key_size, size_t num_buckets) {
	u_int hv = 0;

	if (key) {
		if (ht->flags & CFUHASH_IGNORE_CASE) {
			char *lc_key = (char *)hash_key_dup_lower_case(key, key_size);
			hv = ht->hash_func(lc_key, key_size);
			free(lc_key);
		} else {
			hv = ht->hash_func(key, key_size);
		}
	}

	/* The idea is the following: if, e.g., num_buckets is 32
	   (000001), num_buckets - 1 will be 31 (111110). The & will make
	   sure we only get the first 5 bits which will guarantee the
	   index is less than 32.
	*/
	return hv & (num_buckets - 1);
}

static cfuhash_table_t *
_cfuhash_new(size_t size, u_int32_t flags) {
	cfuhash_table_t *ht;
	
	size = hash_size(size);
	ht = (cfuhash_table_t *)malloc(sizeof(cfuhash_table_t));
	memset(ht, '\000', sizeof(cfuhash_table_t));

	ht->type = libcfu_t_hash_table;
	ht->num_buckets = size;
	ht->entries = 0;
	ht->flags = flags;
	ht->buckets = (cfuhash_entry **)calloc(size, sizeof(cfuhash_entry *));
	pthread_mutex_init(&ht->mutex, NULL);
	
	ht->hash_func = hash_func;
	ht->high = 0.75;
	ht->low = 0.25;
	
	return ht;
}

extern cfuhash_table_t *
cfuhash_new() {
	return _cfuhash_new(8, CFUHASH_FROZEN_UNTIL_GROWS);
}

extern cfuhash_table_t *
cfuhash_new_with_initial_size(size_t size) {
	if (size == 0) size = 8;
	return _cfuhash_new(size, CFUHASH_FROZEN_UNTIL_GROWS);
}

extern cfuhash_table_t *
cfuhash_new_with_flags(u_int32_t flags) {
	return _cfuhash_new(8, CFUHASH_FROZEN_UNTIL_GROWS|flags);
}

extern cfuhash_table_t * cfuhash_new_with_free_fn(cfuhash_free_fn_t ff) {
	cfuhash_table_t *ht = _cfuhash_new(8, CFUHASH_FROZEN_UNTIL_GROWS);
	cfuhash_set_free_function(ht, ff);
	return ht;
}

extern int
cfuhash_copy(cfuhash_table_t *src, cfuhash_table_t *dst) {
	size_t num_keys = 0;
	void **keys = NULL;
	size_t *key_sizes;
	size_t i = 0;
	void *val = NULL;
	size_t data_size = 0;

	keys = cfuhash_keys_data(src, &num_keys, &key_sizes, 0);

	for (i = 0; i < num_keys; i++) {
		if (cfuhash_get_data(src, (void *)keys[i], key_sizes[i], &val, &data_size)) {
			cfuhash_put_data(dst, (void *)keys[i], key_sizes[i], val, data_size, NULL);
		}
		free(keys[i]);
	}

	free(keys);
	free(key_sizes);

	return 1;
}

extern cfuhash_table_t *
cfuhash_merge(cfuhash_table_t *ht1, cfuhash_table_t *ht2, u_int32_t flags) {
	cfuhash_table_t *new_ht = NULL;

	flags |= CFUHASH_FROZEN_UNTIL_GROWS;
	new_ht = _cfuhash_new(cfuhash_num_entries(ht1) + cfuhash_num_entries(ht2), flags);
	if (ht1) cfuhash_copy(ht1, new_ht);
	if (ht2) cfuhash_copy(ht2, new_ht);
	
	return new_ht;
}

/* returns the flags */
extern u_int32_t
cfuhash_get_flags(cfuhash_table_t *ht) {
	return ht->flags;
}

/* sets the given flag and returns the old flags value */
extern u_int32_t
cfuhash_set_flag(cfuhash_table_t *ht, u_int32_t new_flag) {
	u_int32_t flags = ht->flags;
	ht->flags = flags | new_flag;
	return flags;
}

extern u_int32_t
cfuhash_clear_flag(cfuhash_table_t *ht, u_int32_t new_flag) {
	u_int32_t flags = ht->flags;
	ht->flags = flags & ~new_flag;
	return flags;
}

extern int
cfuhash_set_thresholds(cfuhash_table_t *ht, float low, float high) {
	float h = high < 0 ? ht->high : high;
	float l = low < 0 ? ht->low : low;

	if (h < l) return -1;

	ht->high = h;
	ht->low = l;

	return 0;
}

/* Sets the hash function for the hash table ht.  Pass NULL for hf to reset to the default */
extern int
cfuhash_set_hash_function(cfuhash_table_t *ht, cfuhash_function_t hf) {
	/* can't allow changing the hash function if the hash already contains entries */
	if (ht->entries) return -1;
	
	ht->hash_func = hf ? hf : hash_func;
	return 0;
}

extern int
cfuhash_set_free_function(cfuhash_table_t * ht, cfuhash_free_fn_t ff) {
	if (ff) ht->free_fn = ff;
	return 0;
}

static inline void
lock_hash(cfuhash_table_t *ht) {
	if (!ht) return;
	if (ht->flags & CFUHASH_NO_LOCKING) return;
	pthread_mutex_lock(&ht->mutex);
}

static inline void
unlock_hash(cfuhash_table_t *ht) {
	if (!ht) return;
	if (ht->flags & CFUHASH_NO_LOCKING) return;
	pthread_mutex_unlock(&ht->mutex);
}

extern int
cfuhash_lock(cfuhash_table_t *ht) {
	pthread_mutex_lock(&ht->mutex);
	return 1;
}

extern int
cfuhash_unlock(cfuhash_table_t *ht) {
	pthread_mutex_unlock(&ht->mutex);
	return 1;
}

/* see if this key matches the one in the hash entry */
/* uses the convention that zero means a match, like memcmp */

static inline int
hash_cmp(const void *key, size_t key_size, cfuhash_entry *he, u_int case_insensitive) {
	if (key_size != he->key_size) return 1;
	if (key == he->key) return 0;
	if (case_insensitive) {
		return strncasecmp(key, he->key, key_size);
	}
	return memcmp(key, he->key, key_size);
}

static inline cfuhash_entry *
hash_add_entry(cfuhash_table_t *ht, u_int hv, const void *key, size_t key_size,
	void *data, size_t data_size) {
	cfuhash_entry *he = (cfuhash_entry *)calloc(1, sizeof(cfuhash_entry));

	assert(hv < ht->num_buckets);

	if (ht->flags & CFUHASH_NOCOPY_KEYS)
		he->key = (void *)key;
	else
		he->key = hash_key_dup(key, key_size);
	he->key_size = key_size;
	he->data = data;
	he->data_size = data_size;
	he->next = ht->buckets[hv];
	ht->buckets[hv] = he;
	ht->entries++;

	return he;
}

/*
 Returns one if the entry was found, zero otherwise.  If found, r is
 changed to point to the data in the entry.
*/
extern int
cfuhash_get_data(cfuhash_table_t *ht, const void *key, size_t key_size, void **r,
	size_t *data_size) {
	u_int hv = 0;
	cfuhash_entry *hr = NULL;

	if (!ht) return 0;

	if (key_size == (size_t)(-1)) {
		if (key) key_size = strlen(key) + 1;
		else key_size = 0;
	   
	}

	lock_hash(ht);
	hv = hash_value(ht, key, key_size, ht->num_buckets);

	assert(hv < ht->num_buckets);

	for (hr = ht->buckets[hv]; hr; hr = hr->next) {
		if (!hash_cmp(key, key_size, hr, ht->flags & CFUHASH_IGNORE_CASE)) break;
	}

	if (hr && r) {
		*r = hr->data;
		if (data_size) *data_size = hr->data_size;
	}

	unlock_hash(ht);
	
	return (hr ? 1 : 0);
}

/*
 Assumes the key is a null-terminated string, returns the data, or NULL if not found.  Note that it is possible for the data itself to be NULL
*/
extern void *
cfuhash_get(cfuhash_table_t *ht, const char *key) {
	void *r = NULL;
	int rv = 0;
	
	rv = cfuhash_get_data(ht, (const void *)key, -1, &r, NULL);
	if (rv) return r; /* found */
	return NULL;
}

/* Returns 1 if an entry exists in the table for the given key, 0 otherwise */
extern int
cfuhash_exists_data(cfuhash_table_t *ht, const void *key, size_t key_size) {
	void *r = NULL;
	int rv = cfuhash_get_data(ht, key, key_size, &r, NULL);
	if (rv) return 1; /* found */
	return 0;
}

/* Same as cfuhash_exists_data(), except assumes key is a null-terminated string */
extern int
cfuhash_exists(cfuhash_table_t *ht, const char *key) {
	return cfuhash_exists_data(ht, (const void *)key, -1);
}

/*
 Add the entry to the hash.  If there is already an entry for the
 given key, the old data value will be returned in r, and the return
 value is zero.  If a new entry is created for the key, the function
 returns 1.
*/
extern int
cfuhash_put_data(cfuhash_table_t *ht, const void *key, size_t key_size, void *data,
	size_t data_size, void **r) {
	u_int hv = 0;
	cfuhash_entry *he = NULL;
	int added_an_entry = 0;
	
	if (key_size == (size_t)(-1)) {
		if (key) key_size = strlen(key) + 1;
		else key_size = 0;
	}
	if (data_size == (size_t)(-1)) {
		if (data) data_size = strlen(data) + 1;
		else data_size = 0;
		
	}

	lock_hash(ht);
	hv = hash_value(ht, key, key_size, ht->num_buckets);
	assert(hv < ht->num_buckets);
	for (he = ht->buckets[hv]; he; he = he->next) {
		if (!hash_cmp(key, key_size, he, ht->flags & CFUHASH_IGNORE_CASE)) break;
	}

	if (he) {
		if (r) *r = he->data;
		if (ht->free_fn) {
			ht->free_fn(he->data);
			if (r) *r = NULL; /* don't return a pointer to a free()'d location */
		}
		he->data = data;
		he->data_size = data_size;
	} else {
		hash_add_entry(ht, hv, key, key_size, data, data_size);
		added_an_entry = 1;
	}

	unlock_hash(ht);	

	if (added_an_entry && !(ht->flags & CFUHASH_FROZEN)) {
		if ( (float)ht->entries/(float)ht->num_buckets > ht->high ) cfuhash_rehash(ht);
	}

	return added_an_entry;
}

/*
 Same as cfuhash_put_data(), except the key is assumed to be a
 null-terminated string, and the old value is returned if it existed,
 otherwise NULL is returned.
*/
extern void *
cfuhash_put(cfuhash_table_t *ht, const char *key, void *data) {
	void *r = NULL;
	if (!cfuhash_put_data(ht, (const void *)key, -1, data, 0, &r)) {
		return r;
	}
	return NULL;
}

extern void
cfuhash_clear(cfuhash_table_t *ht) {
	cfuhash_entry *he = NULL;
	cfuhash_entry *hep = NULL;
	size_t i = 0;

	lock_hash(ht);
	for (i = 0; i < ht->num_buckets; i++) {
		if ( (he = ht->buckets[i]) ) {
			while (he) {
				hep = he;
				he = he->next;
				if (! (ht->flags & CFUHASH_NOCOPY_KEYS) ) free(hep->key);
				if (ht->free_fn) ht->free_fn(hep->data);
				free(hep);
			}
			ht->buckets[i] = NULL;
		}
	}
	ht->entries = 0;

	unlock_hash(ht);

	if ( !(ht->flags & CFUHASH_FROZEN) &&
		!( (ht->flags & CFUHASH_FROZEN_UNTIL_GROWS) && !ht->resized_count) ) {
		if ( (float)ht->entries/(float)ht->num_buckets < ht->low ) cfuhash_rehash(ht);
	}

}

extern void *
cfuhash_delete_data(cfuhash_table_t *ht, const void *key, size_t key_size) {
	u_int hv = 0;
	cfuhash_entry *he = NULL;
	cfuhash_entry *hep = NULL;
	void *r = NULL;

	if (key_size == (size_t)(-1)) key_size = strlen(key) + 1;
	lock_hash(ht);
	hv = hash_value(ht, key, key_size, ht->num_buckets);

	for (he = ht->buckets[hv]; he; he = he->next) {
		if (!hash_cmp(key, key_size, he, ht->flags & CFUHASH_IGNORE_CASE)) break;
		hep = he;
	}

	if (he) {
		r = he->data;
		if (hep) hep->next = he->next;
		else ht->buckets[hv] = he->next;

		ht->entries--;
		if (! (ht->flags & CFUHASH_NOCOPY_KEYS) ) free(he->key);
		if (ht->free_fn) {
			ht->free_fn(he->data);
			r = NULL; /* don't return a pointer to a free()'d location */
		}
		free(he);
	}

	unlock_hash(ht);

	if (he && !(ht->flags & CFUHASH_FROZEN) &&
		!( (ht->flags & CFUHASH_FROZEN_UNTIL_GROWS) && !ht->resized_count) ) {
		if ( (float)ht->entries/(float)ht->num_buckets < ht->low ) cfuhash_rehash(ht);
	}


	return r;
}

extern void *
cfuhash_delete(cfuhash_table_t *ht, const char *key) {
	return cfuhash_delete_data(ht, key, -1);
}

extern void **
cfuhash_keys_data(cfuhash_table_t *ht, size_t *num_keys, size_t **key_sizes, int fast) {
	size_t *key_lengths = NULL;
	void **keys = NULL;
	cfuhash_entry *he = NULL;
	size_t bucket = 0;
	size_t entry_index = 0;
	size_t key_count = 0;

	if (!ht) {
		*key_sizes = NULL;
		*num_keys = 0;
		return NULL;
	}

	if (! (ht->flags & CFUHASH_NO_LOCKING) ) lock_hash(ht);

	if (key_sizes) key_lengths = (size_t *)calloc(ht->entries, sizeof(size_t));
	keys = (void **)calloc(ht->entries, sizeof(void *));
	
	for (bucket = 0; bucket < ht->num_buckets; bucket++) {
		if ( (he = ht->buckets[bucket]) ) {
			for (; he; he = he->next, entry_index++) {
				if (entry_index >= ht->entries) break; /* this should never happen */

				if (fast) {
					keys[entry_index] = he->key;
				} else {
					keys[entry_index] = (void *)calloc(he->key_size, 1);
					memcpy(keys[entry_index], he->key, he->key_size);
				}
				key_count++;

				if (key_lengths) key_lengths[entry_index] = he->key_size;
			}
		}
	}

	if (! (ht->flags & CFUHASH_NO_LOCKING) ) unlock_hash(ht);

	if (key_sizes) *key_sizes = key_lengths;
	*num_keys = key_count;

	return keys;
}

extern void **
cfuhash_keys(cfuhash_table_t *ht, size_t *num_keys, int fast) {
	return cfuhash_keys_data(ht, num_keys, NULL, fast);
}

extern int
cfuhash_each_data(cfuhash_table_t *ht, void **key, size_t *key_size, void **data,
	size_t *data_size) {

	ht->each_bucket_index = -1;
	ht->each_chain_entry = NULL;

	return cfuhash_next_data(ht, key, key_size, data, data_size);
}

extern int
cfuhash_next_data(cfuhash_table_t *ht, void **key, size_t *key_size, void **data,
	size_t *data_size) {
	
	if (ht->each_chain_entry && ht->each_chain_entry->next) {
		ht->each_chain_entry = ht->each_chain_entry->next;
	} else {
		ht->each_chain_entry = NULL;
		ht->each_bucket_index++;
		for (; ht->each_bucket_index < ht->num_buckets; ht->each_bucket_index++) {
			if (ht->buckets[ht->each_bucket_index]) {
				ht->each_chain_entry = ht->buckets[ht->each_bucket_index];
				break;
			}
		}
	}

	if (ht->each_chain_entry) {
		*key = ht->each_chain_entry->key;
		*key_size = ht->each_chain_entry->key_size;
		*data = ht->each_chain_entry->data;
		if (data_size) *data_size = ht->each_chain_entry->data_size;
		return 1;
	}

	return 0;
}

static void
_cfuhash_destroy_entry(cfuhash_table_t *ht, cfuhash_entry *he, cfuhash_free_fn_t ff) {
	if (ff) {
		ff(he->data);
	} else {
		if (ht->free_fn) ht->free_fn(he->data);
		else {
			if (ht->flags & CFUHASH_FREE_DATA) free(he->data);
		}
	}
	if ( !(ht->flags & CFUHASH_NOCOPY_KEYS) ) free(he->key);
	free(he);
}

extern size_t
cfuhash_foreach_remove(cfuhash_table_t *ht, cfuhash_remove_fn_t r_fn, cfuhash_free_fn_t ff,
					   void *arg) {
	cfuhash_entry *entry = NULL;
	cfuhash_entry *prev = NULL;
	size_t hv = 0;
	size_t num_removed = 0;
	cfuhash_entry **buckets = NULL;
	size_t num_buckets = 0;
	
	if (!ht) return 0;

	lock_hash(ht);

	buckets = ht->buckets;
	num_buckets = ht->num_buckets;
	for (hv = 0; hv < num_buckets; hv++) {
		entry = buckets[hv];
		if (!entry) continue;
		prev = NULL;

		while (entry) {
			if (r_fn(entry->key, entry->key_size, entry->data, entry->data_size, arg)) {
				num_removed++;
				if (prev) {
					prev->next = entry->next;
					_cfuhash_destroy_entry(ht, entry, ff);
					entry = prev->next;
				} else {
					buckets[hv] = entry->next;
					_cfuhash_destroy_entry(ht, entry, NULL);
					entry = buckets[hv];
				}
			} else {
				prev = entry;
				entry = entry->next;
			}
		}
	}

	unlock_hash(ht);

	return num_removed;
}

extern size_t
cfuhash_foreach(cfuhash_table_t *ht, cfuhash_foreach_fn_t fe_fn, void *arg) {
	cfuhash_entry *entry = NULL;
	size_t hv = 0;
	size_t num_accessed = 0;
	cfuhash_entry **buckets = NULL;
	size_t num_buckets = 0;
	int rv = 0;
	
	if (!ht) return 0;

	lock_hash(ht);

	buckets = ht->buckets;
	num_buckets = ht->num_buckets;
	for (hv = 0; hv < num_buckets && !rv; hv++) {
		entry = buckets[hv];

		for (; entry && !rv; entry = entry->next) {
			num_accessed++;
			rv = fe_fn(entry->key, entry->key_size, entry->data, entry->data_size, arg);
		}
	}

	unlock_hash(ht);

	return num_accessed;
}

extern int
cfuhash_each(cfuhash_table_t *ht, char **key, void **data) {
	size_t key_size = 0;
	return cfuhash_each_data(ht, (void **)key, &key_size, data, NULL);
}

extern int
cfuhash_next(cfuhash_table_t *ht, char **key, void **data) {
	size_t key_size = 0;
	return cfuhash_next_data(ht, (void **)key, &key_size, data, NULL);
}

extern int
cfuhash_destroy_with_free_fn(cfuhash_table_t *ht, cfuhash_free_fn_t ff) {
	size_t i;
	if (!ht) return 0;

	lock_hash(ht);
	for (i = 0; i < ht->num_buckets; i++) {
		if (ht->buckets[i]) {
			cfuhash_entry *he = ht->buckets[i];
			while (he) {
				cfuhash_entry *hn = he->next;
				_cfuhash_destroy_entry(ht, he, ff);
				he = hn;
			}
		}
	}
	free(ht->buckets);
	unlock_hash(ht);
	pthread_mutex_destroy(&ht->mutex);
	free(ht);

	return 1;
}

extern int
cfuhash_destroy(cfuhash_table_t *ht) {
	return cfuhash_destroy_with_free_fn(ht, NULL);
}

typedef struct _pretty_print_arg {
	size_t count;
	FILE *fp;
} _pretty_print_arg;

static int
_pretty_print_foreach(void *key, size_t key_size, void *data, size_t data_size, void *arg) {
	_pretty_print_arg *parg = (_pretty_print_arg *)arg;
	key_size = key_size;
	data_size = data_size;
	parg->count += fprintf(parg->fp, "\t\"%s\" => \"%s\",\n", (char *)key, (char *)data);
	return 0;
}

extern int
cfuhash_pretty_print(cfuhash_table_t *ht, FILE *fp) {
	int rv = 0;
	_pretty_print_arg parg;

	parg.fp = fp;
	parg.count = 0;

	rv += fprintf(fp, "{\n");
	
	cfuhash_foreach(ht, _pretty_print_foreach, (void *)&parg);
	rv += parg.count;

	rv += fprintf(fp, "}\n");

	return rv;
}

extern int
cfuhash_rehash(cfuhash_table_t *ht) {
	size_t new_size, i;
	cfuhash_entry **new_buckets = NULL;

	lock_hash(ht);
	new_size = hash_size(ht->entries * 2 / (ht->high + ht->low));
	if (new_size == ht->num_buckets) {
		unlock_hash(ht);
		return 0;
	}
	new_buckets = (cfuhash_entry **)calloc(new_size, sizeof(cfuhash_entry *));

	for (i = 0; i < ht->num_buckets; i++) {
		cfuhash_entry *he = ht->buckets[i];
		while (he) {
			cfuhash_entry *nhe = he->next;
			u_int hv = hash_value(ht, he->key, he->key_size, new_size);
			he->next = new_buckets[hv];
			new_buckets[hv] = he;
			he = nhe;
		}
	}

	ht->num_buckets = new_size;
	free(ht->buckets);
	ht->buckets = new_buckets;
	ht->resized_count++;

	unlock_hash(ht);
	return 1;
}

extern size_t
cfuhash_num_entries(cfuhash_table_t *ht) {
	if (!ht) return 0;
	return ht->entries;
}

extern size_t
cfuhash_num_buckets(cfuhash_table_t *ht) {
	if (!ht) return 0;
	return ht->num_buckets;
}

extern size_t
cfuhash_num_buckets_used(cfuhash_table_t *ht) {
	size_t i = 0;
	size_t count = 0;

	if (!ht) return 0;

	lock_hash(ht);

	for (i = 0; i < ht->num_buckets; i++) {
		if (ht->buckets[i]) count++;
	}
	unlock_hash(ht);
	return count;
}

extern char *
cfuhash_bencode_strings(cfuhash_table_t *ht) {
	cfustring_t *bencoded = cfustring_new_with_initial_size(16);
	char **keys = NULL;
	size_t num_keys = 0;
	size_t i = 0;
	char len_str[32];
	char *rv = NULL;

	cfustring_append(bencoded, "d");

	keys = (char **)cfuhash_keys(ht, &num_keys, 0);
	for (i = 0; i < num_keys; i++) {
		char *val = NULL;

		snprintf(len_str, 32, "%d:", (keys[i] ? strlen(keys[i]) : 0));
		cfustring_append(bencoded, len_str);
		cfustring_append(bencoded, keys[i]);

		val = (char *)cfuhash_get(ht, keys[i]);
		snprintf(len_str, 32, "%d:", (val ? strlen(val) : 0));
		cfustring_append(bencoded, len_str);
		cfustring_append(bencoded, val);

		free(keys[i]);
	}
	free(keys);

	cfustring_append(bencoded, "e");
	rv = cfustring_get_buffer_copy(bencoded);
	cfustring_destroy(bencoded);

	return rv;
}
