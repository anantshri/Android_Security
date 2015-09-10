/* Creation date: 2005-09-04 12:54:41
 * Authors: Don
 * Change log:
 */

#include "cfu.h"

#ifndef _CFU_OPT_H_
#define _CFU_OPT_H_

CFU_BEGIN_DECLS

struct cfuopt_struct;
typedef struct cfuopt_struct cfuopt_t;

/* Returns a new options context */
extern cfuopt_t *cfuopt_new();

/* Adds to the list of known options. */
extern void cfuopt_add_entry(cfuopt_t *context, const char *opt_str, void *arg_data,
	const char *description, const char *arg_description);

/* Parses the command line and modifies argc and argv to account for
 * left over arguments.
 */
extern void cfuopt_parse(cfuopt_t *context, int *argc, char ***argv, char **error);

/* Returns a help string built from the entries added with
 * cfuopt_add_entry().
 */
extern char * cfuopt_get_help_str(cfuopt_t *context);

/* Frees up resources used by the option parser. */
extern void cfuopt_destroy(cfuopt_t *context);

CFU_END_DECLS

#endif
