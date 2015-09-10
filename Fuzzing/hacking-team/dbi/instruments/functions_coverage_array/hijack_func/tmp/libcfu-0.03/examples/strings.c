/* Creation date: 2005-06-27 22:19:17
 * Authors: Don
 * Change log:
 */

#include "cfustring.h"
#include <stdio.h>
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


int main(int argc, char **argv) {
	cfustring_t *buf = cfustring_new_with_initial_size(32);
	char **strings = NULL;
	size_t num_strings = 0;
	size_t num_headers = 0;
	size_t i = 0;
	char *ref_addr = NULL;
	char *s = NULL;
	cfustring_t *header_block = cfustring_new_from_string(
		"User-Agent: curl/7.10.6 (i386-redhat-linux-gnu) libcurl/7.10.6 OpenSSL/0.9.7a ipv6 zlib/1.2.0.7\r\n"
		"Host: localhost:9095\r\n"
		"Pragma: no-cache\r\n"
		"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, */*\r\n");
	cfustring_t **header_lines = NULL;

	argc = argc; argv = argv; /* avoid compiler warnings */

	cfustring_append(buf, "test^*string%2^*3");
	strings = cfustring_split_to_c_str(buf, &num_strings, 0, "%", "^*", NULL);

	printf("got back %u strings\n", num_strings);

	printf("\n");
	for (i = 0; i < num_strings; i++) {
		printf("'%s'\n", strings[i]);
	}

	for (i = 0; i < num_strings; i++) {
		free(strings[i]);
	}
	free(strings);

	header_lines = cfustring_split(header_block, &num_headers, 0, "\r\n", "\n", NULL);
	printf("got back %u headers\n", num_strings);

	for (i = 0; i < num_headers; i++) {
		strings = cfustring_split_to_c_str(header_lines[i], &num_strings, 2, ":", NULL);
		if (num_strings != 2) {
			/* bad header */
			size_t j = 0;
			for (j = 0; j < num_strings; j++) free(strings[j]);
			free(strings);
			fprintf(stderr, "bad header: %u strings from split -- '%s'\n", num_strings, cfustring_get_buffer(header_lines[i]));
			continue;
		}

		printf("Got header '%s' => '%s'\n", strings[0], strings[1]);
		free(strings[0]); free(strings[1]);
		free(strings);
	}

	for (i = 0; i < num_headers; i++) {
		assert(cfu_is_string(header_lines[i]));
		cfustring_destroy(header_lines[i]);
	}
	free(header_lines);

	cfustring_sprintf(buf, "this %% is a string - '%.2f'", 509.0);
	printf("cfustring_sprintf(): '%s'\n", cfustring_get_buffer(buf));

	cfustring_destroy(buf);

	s = cfustring_sprintf_c_str("this is a test with a num: '%u'\n", 5);

	printf("got: %s\n", s);
	free(s);

	ref_addr = cfustring_sprintf_c_str("%p", (void *)header_block);

	cfustring_destroy(header_block);

	printf("\n\nheader_block as ref_addr='%s'\n\n", ref_addr);
	free(ref_addr);

	
	
	return 0;
}
