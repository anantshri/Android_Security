/* Creation date: 2005-06-25 10:33:53
 * Authors: Don
 * Change log:
 */

#include "cfuhash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int
remove_func(void *key, size_t key_size, void *data, size_t data_size, void *arg) {
	data_size = data_size;
	data = data;
	arg = arg;

	if (key_size > 7) {
		if (!strncasecmp(key, "content", 7)) {
			return 1;
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	cfuhash_table_t *hash = cfuhash_new_with_initial_size(30);
	cfuhash_table_t *hash2 = cfuhash_new_with_initial_size(4);
	char *val = NULL;
	char list[32][2][32];
	size_t i;

	argc = argc;
	argv = argv;

	cfuhash_set_flag(hash, CFUHASH_FROZEN_UNTIL_GROWS);

	for (i = 0; i < 32; i++) {
		sprintf(list[i][0], "test_var%d", i);
		sprintf(list[i][1], "value%d", i);
	}
	
	cfuhash_put(hash, "var1", "value1");
	cfuhash_put(hash, "var2", "value2");
	cfuhash_put(hash, "var3", "value3");
	cfuhash_put(hash, "var4", "value4");

	cfuhash_pretty_print(hash, stdout);

	printf("\n\n");
	val = (char *)cfuhash_delete(hash, "var3");
	printf("delete: got back '%s'\n\n", val);

	cfuhash_pretty_print(hash, stdout);

	printf("\n\n");
	val = cfuhash_get(hash, "var2");
	printf("got var2='%s'\n", val);
	printf("var4 %s\n", cfuhash_exists(hash, "var4") ? "exists" : "does NOT exist!!!");

	printf("%d entries, %d buckets used out of %d\n", cfuhash_num_entries(hash), cfuhash_num_buckets_used(hash), cfuhash_num_buckets(hash));

	cfuhash_pretty_print(hash, stdout);
	
	cfuhash_clear(hash);

	for (i = 0; i < 32; i++) {
		size_t used = cfuhash_num_buckets_used(hash);
		size_t num_buckets = cfuhash_num_buckets(hash);
		size_t num_entries = cfuhash_num_entries(hash);
		cfuhash_put(hash, list[i][0], list[i][1]);
		printf("%d entries, %d buckets used out of %d (%.2f)\n", num_entries, used, num_buckets, (float)num_entries/(float)num_buckets);

	}

	cfuhash_pretty_print(hash, stdout);

	{
		char **keys = NULL;
		size_t *key_sizes = NULL;
		size_t key_count = 0;
		
		keys = (char **)cfuhash_keys_data(hash, &key_count, &key_sizes, 0);

		printf("\n\nkeys (%u):\n", key_count);
		for (i = 0; i < key_count; i++) {
			printf("\t%s\n", keys[i]);
			free(keys[i]);
		}
		free(keys);
		free(key_sizes);
	}

	cfuhash_clear(hash);
	printf("%d entries, %d buckets used out of %d\n", cfuhash_num_entries(hash), cfuhash_num_buckets_used(hash), cfuhash_num_buckets(hash));

	cfuhash_destroy(hash);

	printf("\n\n====> case-insensitive hash:\n\n");
	hash = cfuhash_new_with_initial_size(30);
	cfuhash_set_flag(hash, CFUHASH_IGNORE_CASE);
	cfuhash_put(hash, "Content-Type", "value1");
	cfuhash_put(hash, "Content-Length", "value2");
	
	cfuhash_put(hash2, "var3", "value3");
	cfuhash_put(hash2, "var4", "value4");
	

	cfuhash_pretty_print(hash, stdout);
	cfuhash_copy(hash2, hash);
	cfuhash_pretty_print(hash, stdout);

	{
		char keys[4][32] = { "content-type", "content-length", "Var3", "VaR4" };
		size_t i = 0;

		for (i = 0; i < 4; i++) {
			printf("%s => %s\n", keys[i], (char *)cfuhash_get(hash, keys[i]));
		}

		cfuhash_foreach_remove(hash, remove_func, NULL, NULL);
		printf("\n\nafter removing content*:\n");
		for (i = 0; i < 4; i++) {
			printf("%s => %s\n", keys[i], (char *)cfuhash_get(hash, keys[i]));
		}

	}

	cfuhash_destroy(hash);
	cfuhash_destroy(hash2);

	return 0;
}
