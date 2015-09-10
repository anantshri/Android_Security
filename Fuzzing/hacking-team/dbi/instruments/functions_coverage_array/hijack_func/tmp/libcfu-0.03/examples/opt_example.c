/* Creation date: 2005-09-04 14:46:45
 * Authors: Don
 * Change log:
 */

#include "cfuopt.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	cfuopt_t *opt = cfuopt_new();
	int verbose = 0;
	char *file = NULL;
	long count = 0;
	char *error = NULL;
	double scale = 0.0;
	char *help = NULL;
	int i = 0;

	argc = argc;
	argv = argv;

	cfuopt_add_entry(opt, "verbose|v!", &verbose, "Verbosity", "");
	cfuopt_add_entry(opt, "file|f:s", &file, "File to load", "FILE");
	cfuopt_add_entry(opt, "count|c|n=i", &count, "Count to run", "COUNT");
	cfuopt_add_entry(opt, "scale|s:f", &scale, "Scaling factor", "SCALE");

	printf("\nStarting args:\n");
	for (i = 0; i < argc; i++) {
		printf("\t%s\n", argv[i]);
	}

	cfuopt_parse(opt, &argc, &argv, &error);

	printf("\n\n");
	printf("verbose=%d\n", verbose);
	printf("file=%s\n", file);
	printf("count=%ld\n", count);
	printf("scale=%f\n", scale);

	help = cfuopt_get_help_str(opt);

	printf("\n\nhelp:\n%s\n", help);
	free(help);

	printf("\nLeft over args:\n");
	for (i = 0; i < argc; i++) {
		printf("\t%s\n", argv[i]);
	}

	cfuopt_destroy(opt);

	free(file);

	return 0;
}
