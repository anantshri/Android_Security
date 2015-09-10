/* Creation date: 2005-07-08 23:43:37
 * Authors: Don
 * Change log:
 */

#include "cfuconf.h"

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
	char *file_path = "/tmp/test.conf";
	cfuconf_t *conf = NULL;
	char *error = NULL;

	argc = argc;
	argv = argv;

	if (cfuconf_parse_file(file_path, &conf, &error) < 0) {
		fprintf(stderr, "Error loading conf file: %s\n", error);
		free(error);
		exit(-1);
	}

	fprintf(stderr, "\n\nvals:\n\n");

	cfuconf_pretty_print_conf(conf, stderr, 0);

	cfuconf_destroy(conf);

	return 0;
}
