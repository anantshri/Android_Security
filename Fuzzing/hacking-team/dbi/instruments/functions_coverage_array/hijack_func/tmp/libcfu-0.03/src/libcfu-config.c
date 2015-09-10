/* Creation date: 2005-08-07 13:34:49
 * Authors: Don
 * Change log:
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cfu.h"
#include "cfuopt.h"

#define GET_PREFIX CFU_PREFIX
#define GET_EXEC_PREFIX CFU_EXEC_PREFIX
#define GET_VERSION LIBCFU_VERSION
#define GET_LIBS "-L"  CFU_LIBDIR " -lcfu " CFU_LIBS
#define GET_CFLAGS "-I" CFU_INCLUDEDIR

static void
print_usage() {
	fprintf(stderr, "Usage: libcfu-config [OPTIONS]\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "        --prefix         [%s]\n", GET_PREFIX);
	fprintf(stderr, "        --exec-prefix    [%s]\n", GET_EXEC_PREFIX);
	fprintf(stderr, "        --version        [%s]\n", GET_VERSION);
	fprintf(stderr, "        --libs           [%s]\n", GET_LIBS);
	fprintf(stderr, "        --cflags         [%s]\n", GET_CFLAGS);
}

int main(int argc, char **argv) {
	char *arg = NULL;
	int do_prefix = 0;
	int do_exec_prefix = 0;
	int do_version = 0;
	int do_libs = 0;
	int do_cflags = 0;
	cfuopt_t *options = cfuopt_new();
	char *error = NULL;

	cfuopt_add_entry(options, "prefix!", &do_prefix, "Display install prefix", NULL);
	cfuopt_add_entry(options, "exec-prefix!", &do_exec_prefix, "Display install prefix for executables", NULL);
	cfuopt_add_entry(options, "version!", &do_version, "Display library version", NULL);
	cfuopt_add_entry(options, "libs!", &do_libs, "Display libraries required for linking with this library", NULL);
	cfuopt_add_entry(options, "cflags", &do_cflags, "Display CFLAGS required for compilation", NULL);

	if (argc < 2) {
		print_usage();
		exit(1);
	}

	cfuopt_parse(options, &argc, &argv, &error);

	arg = argv[1];

	if (do_prefix) {
		fprintf(stdout, "%s\n", GET_PREFIX);
	} else if (do_exec_prefix) {
		fprintf(stdout, "%s\n", GET_EXEC_PREFIX);
	} else if (do_version) {
		fprintf(stdout, "%s\n", GET_VERSION);
	} else if (do_libs) {
		fprintf(stdout, "%s\n", GET_LIBS);
	} else if (do_cflags) {
		fprintf(stdout, "%s\n", GET_CFLAGS);
	} else {
		print_usage();
		cfuopt_destroy(options);
		exit(1);
	}

	cfuopt_destroy(options);

	return 0;
}
