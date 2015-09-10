/* Creation date: 2005-06-24 21:22:09
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

#ifndef _CFU_HASH_H_
#define _CFU_HASH_H_

#include <cfu.h>

#include <sys/types.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* The hash table itself. */
    struct cfuhash_table;
    typedef struct cfuhash_table cfuhash_table_t;

    /* Prototype for a pointer to a hashing function. */
    typedef u_int32_t (*cfuhash_function_t)(const void *key, size_t length);
    
    /* Prototype for a pointer to a free function. */
    typedef void (*cfuhash_free_fn_t)(void *data);
    
    /* Prototype for a pointer to a function that determines whether
     * or not to remove an entry from the hash.
    */
    typedef int (*cfuhash_remove_fn_t)(void *key, size_t key_size, void *data, size_t data_size,
        void *arg);

    /* Prototype for a pointer to a function to be called foreach
     * key/value pair in the hash by cfuhash_foreach().  Iteration
     * stops if a non-zero value is returned.
	 */
    typedef int (*cfuhash_foreach_fn_t)(void *key, size_t key_size, void *data, size_t data_size,
        void *arg);
    
    /* Creates a new hash table. */
    extern cfuhash_table_t * cfuhash_new();

    /* Creates a new hash table with the specified size (number of
     *  buckets).
     */
    extern cfuhash_table_t * cfuhash_new_with_initial_size(size_t size);

    /* Creates a new hash table with the specified flags.  Pass zero
     *  for flags if you want the defaults.
     */
	extern cfuhash_table_t * cfuhash_new_with_flags(u_int32_t flags);
    
    /* Same as cfuhash_new() except automatically calls cfuhash_set_free_fn(). */
    extern cfuhash_table_t * cfuhash_new_with_free_fn(cfuhash_free_fn_t ff);

    /* Copies entries in src to dst */
    extern int cfuhash_copy(cfuhash_table_t *src, cfuhash_table_t *dst);

    /* Returns a new hash containing entries from both hash tables.
     * For any entries with the same key, the one from ht2 wins.
    */
    extern cfuhash_table_t * cfuhash_merge(cfuhash_table_t *ht1, cfuhash_table_t *ht2,
        u_int32_t flags);

    /* Sets the hashing function to use when computing which bucket to add
     * entries to.  It should return a 32-bit unsigned integer.  By
     * default, Perl's hashing algorithm is used.
     */
    extern int cfuhash_set_hash_function(cfuhash_table_t *ht, cfuhash_function_t hf);

    /* Sets the thresholds for when to rehash.  The ratio
     * num_entries/buckets is compared against low and high.  If it is
     * below 'low' or above 'high', the hash will shrink or grow,
     * respectively, unless the flags say to do otherwise.
     */
    extern int cfuhash_set_thresholds(cfuhash_table_t *ht, float low, float high);

    /* Sets the function to use when removing an entry from the hash,
     * i.e., when replacing an existing entry, deleting an entry, or
     * clearing the hash.  It is passed the value of the entry as a
     * void *.
     */
    extern int cfuhash_set_free_function(cfuhash_table_t * ht, cfuhash_free_fn_t ff);

    /* Returns the hash's flags. See below for flag definitions. */
    extern u_int32_t cfuhash_get_flags(cfuhash_table_t *ht);

    /* Sets a flag. */
    extern u_int32_t cfuhash_set_flag(cfuhash_table_t *ht, u_int32_t flag);

    /* Clears a flag. */
    extern u_int32_t cfuhash_clear_flag(cfuhash_table_t *ht, u_int32_t new_flag);

    /* Returns the value for the entry with given key.  If key_size is -1,
     * key is assumed to be a null-terminated string.  If data_size is not
     * NULL, the size of the value is placed into data_size.
     */
    extern int cfuhash_get_data(cfuhash_table_t *ht, const void *key, size_t key_size,
        void **data, size_t *data_size);

    /* Returns 1 if an entry with the given key exists in the hash, 0 otherwise. */
    extern int cfuhash_exists_data(cfuhash_table_t *ht, const void *key, size_t key_size);

    /* Inserts the given data value into the hash and associates it with
     *  key.  If key_size is -1, key is assumed to be a null-terminated
     *  string.  If data_size is -1, it is assumed to be a null-terminated
     *  string (it's length will be calculated using strlen).  If
     *  data_size is zero, it will be returned as zero when the value is
     *  requested.
     */
    extern int cfuhash_put_data(cfuhash_table_t *ht, const void *key, size_t key_size, void *data,
        size_t data_size, void **r);

    /* Clears the hash table (deletes all entries). */
    extern void cfuhash_clear(cfuhash_table_t *ht);

    /* Deletes the entry in the hash associated with key. If the entry
     * existed, it's value will be returned.
     */
    extern void * cfuhash_delete_data(cfuhash_table_t *ht, const void *key, size_t key_size);

    /* Returns all the keys from the hash.  The number of keys is placed
     * into the value pointed to by num_keys.  If key_sizes is not NULL,
     * it will be set to an array of key sizes.  If fast is zero, copies
     * of the keys are returned.  Otherwise, pointers to the real keys
     * will be returned. 
     */
    extern void **cfuhash_keys_data(cfuhash_table_t *ht, size_t *num_keys, size_t **key_sizes,
        int fast);

    /* Initializes a loop over all the key/value pairs in the hash.  It
     * returns the first key/value pair (see cfuhash_next_data()).  1 is
     * returned if there are any entries in the hash.  0 is returned
     * otherwise.
     */
    extern int cfuhash_each_data(cfuhash_table_t *ht, void **key, size_t *key_size, void **data,
        size_t *data_size);

    /* Gets the next key/value pair from the hash.  You must initialize
     * the loop using cfuhash_each_data() before calling this function.
     * If a entry is left to return, 1 is returned from the function.  0
     * is returned if there are no more entries in the hash.
     */
    extern int cfuhash_next_data(cfuhash_table_t *ht, void **key, size_t *key_size, void **data,
        size_t *data_size);

    /* Iterates over the key/value pairs in the hash, passing each one
     * to r_fn, and removes all entries for which r_fn returns true.
     * If ff is not NULL, it is the passed the data to be freed.  arg
     * is passed to r_fn.
     */
    extern size_t cfuhash_foreach_remove(cfuhash_table_t *ht, cfuhash_remove_fn_t r_fn,
        cfuhash_free_fn_t ff, void *arg);

    /* Iterates over the key/value pairs in the hash, passing each one
     * to fe_fn, along with arg. This locks the hash, so do not call
     * any operations on the hash from within fe_fn unless you really
     * know what you're doing.  A non-zero return value from fe_fn()
     * stops the iteration.
     */
    extern size_t cfuhash_foreach(cfuhash_table_t *ht, cfuhash_foreach_fn_t fe_fn, void *arg);

    /* Frees all resources allocated by the hash.  If ff is not NULL, it
     * is called for each hash entry with the value of the entry passed as
     * its only argument.  If ff is not NULL, it overrides any function
     * set previously with cfuhash_set_free_function().
     */
    extern int cfuhash_destroy(cfuhash_table_t *ht);

    extern int cfuhash_destroy_with_free_fn(cfuhash_table_t *ht, cfuhash_free_fn_t ff);

    /* Rebuild the hash to better accomodate the number of entries. See
     * cfuhash_set_thresholds().
     */
    extern int cfuhash_rehash(cfuhash_table_t *ht);

    /* Returns the number entries in the hash. */
    extern size_t cfuhash_num_entries(cfuhash_table_t *ht);

    /* Returns the number of buckets allocated for the hash. */
    extern size_t cfuhash_num_buckets(cfuhash_table_t *ht);

    /* Returns the number of buckets actually used out of the total number
     * allocated for the hash.
     */
    extern size_t cfuhash_num_buckets_used(cfuhash_table_t *ht);

    /* Assumes all the keys and values are null-terminated strings and
     * returns a bencoded string representing the hash (see
     * http://www.bittorrent.com/protocol.html)
     */
    extern char * cfuhash_bencode_strings(cfuhash_table_t *ht);

    /* Locks the hash.  Use this with the each and next functions for
     * concurrency control.  Note that the hash is locked automatically
     * when doing inserts and deletes, so if you lock the hash and then
     * try to insert something into it, you may get into a deadlock,
     * depending on your system defaults for how mutexes work.
     */
    extern int cfuhash_lock(cfuhash_table_t *ht);

    /* Unlocks the hash.  Use this with the each an next functions for
     * concurrency control.  The caveat for cfuhash_lcok() also applies to
     * this function.
     */
    extern int cfuhash_unlock(cfuhash_table_t *ht);

    /* Pretty print the hash's key/value pairs to the stream fp.  It is
     * assumed that all the keys and values are null-terminated strings.
     */
    extern int cfuhash_pretty_print(cfuhash_table_t *ht, FILE *fp);

    /* These are like the _data versions of these functions, with the
     * following exceptions:
     *   1) They assume that the key provided is a null-terminated string.
     *   2) They don't worry about the size of the data.
     *   3) Returned keys or values are the return value of the function.
     */
    extern void * cfuhash_get(cfuhash_table_t *ht, const char *key);
    extern int cfuhash_exists(cfuhash_table_t *ht, const char *key);
    extern void * cfuhash_put(cfuhash_table_t *ht, const char *key, void *data);
    extern void * cfuhash_delete(cfuhash_table_t *ht, const char *key);
    extern int cfuhash_each(cfuhash_table_t *ht, char **key, void **data);
    extern int cfuhash_next(cfuhash_table_t *ht, char **key, void **data);
    extern void **cfuhash_keys(cfuhash_table_t *ht, size_t *num_keys, int fast);


/* hash table flags */
#define CFUHASH_NOCOPY_KEYS 1        /* do not copy the key when inserting a hash entry */
#define CFUHASH_NO_LOCKING (1 << 1)  /* do not make the hash thread-safe */
#define CFUHASH_FROZEN (1 << 2)      /* do not rehash when the size thresholds are reached */
#define CFUHASH_FROZEN_UNTIL_GROWS (1 << 3) /* do not shrink the hash until it has grown */
#define CFUHASH_FREE_DATA (1 << 4)   /* call free() on each value when the hash is destroyed */
#define CFUHASH_IGNORE_CASE (1 << 5) /* treat keys case-insensitively */


#ifdef __cplusplus
}
#endif

#endif
