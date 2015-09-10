/* Creation date: 2005-06-26 00:41:47
 * Authors: Don
 * Change log:
 */

#include "cfuhash.h"
#include "cfutime.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

static
void free_data(void *data) {
	free(data);
}

static char *
dup_str(const char *str) {
	size_t len = strlen(str);
	char *ns = (char *)calloc(len + 1, 1);
	memcpy(ns, str, len);
	return ns;
}

static u_int32_t
hash_func1(const void *key, size_t length) {
	size_t i = length;
	u_int hash = 0;
	char *s = (char *)key;
	while (i--) hash = hash * 33 + *s++;

	return hash;
}

/* From libtc */
#define mix(a,b,c)				\
{						\
  a -= b; a -= c; a ^= (c>>13);			\
  b -= c; b -= a; b ^= (a<<8);			\
  c -= a; c -= b; c ^= (b>>13);			\
  a -= b; a -= c; a ^= (c>>12);			\
  b -= c; b -= a; b ^= (a<<16);			\
  c -= a; c -= b; c ^= (b>>5);			\
  a -= b; a -= c; a ^= (c>>3);			\
  b -= c; b -= a; b ^= (a<<10);			\
  c -= a; c -= b; c ^= (b>>15);			\
}

typedef u_int32_t ub4;

static u_int
hash_func_tc(const void *key, size_t length)
{
    register ub4 a,b,c,len;
    const char *k = key;

    /* Set up the internal state */
    len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = 0;               /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while(len >= 12){
	a += (k[0] +((ub4)k[1]<<8) +((ub4)k[2]<<16) +((ub4)k[3]<<24));
	b += (k[4] +((ub4)k[5]<<8) +((ub4)k[6]<<16) +((ub4)k[7]<<24));
	c += (k[8] +((ub4)k[9]<<8) +((ub4)k[10]<<16)+((ub4)k[11]<<24));
	mix(a,b,c);
	k += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += length;
    switch(len){              /* all the case statements fall through */
    case 11: c+=((ub4)k[10]<<24);
    case 10: c+=((ub4)k[9]<<16);
    case 9 : c+=((ub4)k[8]<<8);
	/* the first byte of c is reserved for the length */
    case 8 : b+=((ub4)k[7]<<24);
    case 7 : b+=((ub4)k[6]<<16);
    case 6 : b+=((ub4)k[5]<<8);
    case 5 : b+=k[4];
    case 4 : a+=((ub4)k[3]<<24);
    case 3 : a+=((ub4)k[2]<<16);
    case 2 : a+=((ub4)k[1]<<8);
    case 1 : a+=k[0];
	/* case 0: nothing left to add */
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
}

/* End of code from Jenkins */


static int
time_it(cfuhash_function_t hf, double *elapsed_time, u_int32_t num_tests) {
	cfuhash_table_t *hash = cfuhash_new_with_initial_size(30);
	u_int32_t flags = 0;
	char key[32];
	char value[32];
	size_t i;
	size_t used;
	size_t num_buckets;
	size_t num_entries;
	cfutime_t *time = cfutime_new();

	/* freeze the hash so that it won't shrink while we put in all the data */
	flags = cfuhash_set_flag(hash, CFUHASH_FROZEN_UNTIL_GROWS);
	cfuhash_set_hash_function(hash, hf);

	cfutime_begin(time);
	for (i = 0; i < num_tests; i++) {
		sprintf(key, "%u", 15000000 - i);
		sprintf(value, "value%d", i);
		cfuhash_put(hash, key, dup_str(value));
	}
	cfutime_end(time);
	*elapsed_time = cfutime_elapsed(time);

	used = cfuhash_num_buckets_used(hash);
	num_buckets = cfuhash_num_buckets(hash);
	num_entries = cfuhash_num_entries(hash);
	printf("%d entries, %d/%d buckets (%.2f%%), %.2f%% threshold check\n", num_entries, used, num_buckets, 100.0 * (float)used/(float)num_buckets, 100.0 * (float)num_entries/(float)num_buckets);

	cfuhash_destroy_with_free_fn(hash, free_data);	

	return 0;
}


int main(int argc, char **argv) {
	double elapsed_time = 0;
	u_int32_t num_tests = 3000000;

	argc = argc;
	argv = argv;

	printf("mutex is %d bytes\n", sizeof(pthread_mutex_t));

	printf("default:\n");
	time_it(NULL, &elapsed_time, num_tests);
	printf("%.3f seconds\n", elapsed_time);
	printf("\n");
	fflush(stdout);

	printf("hash_func1:\n");
	time_it(hash_func1, &elapsed_time, num_tests);
	printf("%.3f seconds\n", elapsed_time);
	printf("\n");
	fflush(stdout);

	printf("hash_func_tc:\n");
	time_it(hash_func_tc, &elapsed_time, num_tests);
	printf("%.3f seconds\n", elapsed_time);
	printf("\n");
	fflush(stdout);

	return 1;
}
