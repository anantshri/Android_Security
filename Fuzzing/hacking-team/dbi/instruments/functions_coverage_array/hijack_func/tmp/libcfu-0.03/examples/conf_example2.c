/* Creation date: 2005-07-25 22:00:49
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
	cfuconf_t *conf = NULL;
	char *error = NULL;
	char *buffer =  "SetVar Test1 Val1\n"
		"SetVar Test2 Val2\n"
		"<test_container c1>\n"
		"    SetVar Test3 Val3\n"
		"    SetVar Test4 Val4\n"
		"</test_container>\n";

	argc = argc;
	argv = argv;

	if (cfuconf_parse_buffer(buffer, &conf, &error) < 0) {
		fprintf(stderr, "Error loading conf buffer: %s\n", error);
		free(error);
		exit(-1);
	}

	fprintf(stderr, "\n\nvals:\n\n");

	cfuconf_pretty_print_conf(conf, stderr, 0);

	cfuconf_destroy(conf);

	return 0;
}
