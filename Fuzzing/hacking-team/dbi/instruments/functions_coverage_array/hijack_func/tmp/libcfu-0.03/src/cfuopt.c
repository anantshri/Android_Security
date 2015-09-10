/* Creation date: 2005-09-04 12:57:25
 * Authors: Don
 * Change log:
 */

#include "cfu.h"
#include "cfuopt.h"
#include "cfuhash.h"
#include "cfulist.h"
#include "cfustring.h"

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

struct cfuopt_struct {
	libcfu_type type;
	cfulist_t *option_list;
	cfuhash_table_t *option_map;
	cfulist_t *extra;
	char *progname;
};

typedef enum {
	cfuopt_arg_invalid = 0,
	cfuopt_arg_bool,
	cfuopt_arg_string,
	cfuopt_arg_int,
	cfuopt_arg_float,
	cfuopt_arg_string_array
} cfuopt_arg_t;

typedef struct cfuopt_list_entry {
	const char *arg_data;
	const char *description;
	const char *arg_description;
	cfuopt_arg_t arg_type;
	int required;
	cfulist_t *param_names;
} cfuopt_list_entry_t;

#define CFU_OPT_ALLOC(type, count) (type *)calloc(count, sizeof(type));

extern cfuopt_t *
cfuopt_new() {
	cfuopt_t *context = (cfuopt_t *)calloc(1, sizeof(cfuopt_t));
	context->option_list = cfulist_new();
	context->option_map = cfuhash_new();
	context->extra = cfulist_new();
	return context;
}

#define CFUOPT_SET_TYPE(spec, type_var) \
	switch (spec) { \
	  case 's': \
		  type_var = cfuopt_arg_string; \
		  break; \
	  case 'i': \
		  type_var = cfuopt_arg_int; \
		  break; \
	  case 'f': \
		  type_var = cfuopt_arg_float; \
		  break; \
	  case '!': \
		  type_var = cfuopt_arg_bool; \
	  default: \
		  break; \
	}


/*
  Parse a string like Perl's Getopt::Long takes, e.g.

  "verbose|v!",
  "file|f=s",
  "scale:f",
  "count:i"

  ! means boolean flag
  = means required
  : means optional
  s means arbitrary string (C string)
  f means float (double)
  i means integer (long)

*/
static void
parse_opt_str(const char *opt_str, cfulist_t **param_list, cfuopt_arg_t *type, int *is_required) {
	cfulist_t *params = cfulist_new();
	size_t num_names = 0;
	char **opt_names = cfustring_c_str_split(opt_str, &num_names, 0, "|", NULL);
	char *last_opt = NULL;
	char *pos = NULL;
	size_t opt_len = 0;
	cfuopt_arg_t arg_type = cfuopt_arg_invalid;
	int required = 0;
	size_t i = 0;

	if (num_names == 0) {
		cfulist_destroy(params);
		free(opt_names);
		return;
	}

	last_opt = opt_names[num_names - 1];
	opt_len = strlen(last_opt);
	if ( (pos = memchr((void *)last_opt, '=', opt_len)) ) {
		char *tmp = pos;
		required = 1;
		tmp++;
		if (*tmp) {
			CFUOPT_SET_TYPE(*tmp, arg_type);
			*pos = '\000';
		}
		else {
			arg_type = cfuopt_arg_invalid;
		}
	}
	else if ( (pos = memchr((void *)last_opt, ':', opt_len)) ) {
		char *tmp = pos;
		required = 0;
		tmp++;
		if (*tmp) {
			CFUOPT_SET_TYPE(*tmp, arg_type);
			*pos = '\000';
		}
		else {
			arg_type = cfuopt_arg_invalid;
		}
	}
	else if ( (pos = memchr((void *)last_opt, '!', opt_len)) ) {
		required = 0;
		arg_type = cfuopt_arg_bool;
		*pos = '\000';
	}

	for (i = 0; i < num_names; i++) {
		cfulist_push(params, opt_names[i]);
	}

	free(opt_names);
	*type = arg_type;
	*param_list = params;
	*is_required = required;
}

static void
_simple_list_free_fn(void *data) {
	free(data);
}

struct _add_entry_struct {
	cfuopt_t *context;
	cfuopt_list_entry_t *entry;
};

static int
_add_to_option_map(void *data, size_t data_size, void *arg) {
	struct _add_entry_struct *es = (struct _add_entry_struct *)arg;
	data_size = data_size;

	cfuhash_put(es->context->option_map, (char *)data, (void *)es->entry);
	
	return 0;
}

extern void
cfuopt_add_entry(cfuopt_t *context, const char *opt_str, void *arg_data,
	const char *description, const char *arg_description) {
	cfuopt_list_entry_t *entry = CFU_OPT_ALLOC(cfuopt_list_entry_t, 1);
	cfulist_t *param_list = NULL;
	cfuopt_arg_t arg_type = cfuopt_arg_invalid;
	struct _add_entry_struct entry_struct;
	int required = 0;

	parse_opt_str(opt_str, &param_list, &arg_type, &required);

	entry->arg_data = arg_data;
	entry->description = description;
	entry->arg_description = arg_description;
	entry->arg_type = arg_type;
	entry->required = required;
	entry->param_names = param_list;

	cfulist_push(context->option_list, (void *)entry);
	
	entry_struct.entry = entry;
	entry_struct.context = context;

	cfulist_foreach(param_list, _add_to_option_map, &entry_struct);
}

static void
check_arg(const char *arg, int *is_long, int *is_short, int *is_data, int *is_end_of_options,
	char **parsed_arg, const char **value) {
	const char *ptr = NULL;

	ptr = arg;
	*is_long = 0;
	*is_short = 0;
	*is_data = 0;
	*is_end_of_options = 0;
	*parsed_arg = NULL;
	*value = NULL;

	if (!ptr || !*ptr) return;
	
	if (*ptr != '-') {
		*is_data = 1;
		*parsed_arg = cfustring_dup_c_str(ptr);
		return;
	}
	ptr++;
	if (!*ptr) {
		*is_data = 1;
		*parsed_arg = cfustring_dup_c_str(ptr - 1);
		return;
	}
	if (*ptr == '-') {
		const char *val_ptr = NULL;
		ptr++;
		if (!*ptr) {
			*is_end_of_options = 1;
			return;
		}
		*is_long = 1;
		*parsed_arg = cfustring_dup_c_str(ptr);
		val_ptr = ptr;
		for (val_ptr = ptr; *val_ptr && *val_ptr != '='; val_ptr++);
		if (*val_ptr == '=') {
			(*parsed_arg)[val_ptr - ptr] = '\000';
			val_ptr++;
			*value = val_ptr;
		}
		return;
	} else {
		*is_short = 1;
		*parsed_arg = cfustring_dup_c_str(ptr);
		return;
	}
}

static void
_set_entry_val(cfuopt_list_entry_t *entry, const char *value) {
	switch (entry->arg_type) {
	  case cfuopt_arg_bool:
		  if (entry->arg_data) *((int *)entry->arg_data) = 1;
		  break;
	  case cfuopt_arg_int:
		  if (entry->arg_data) *((int *)entry->arg_data) = atol((char *)value);
		  break;
	  case cfuopt_arg_float:
		  if (entry->arg_data) *((double *)entry->arg_data) = atof((char *)value);
		  break;
	  case cfuopt_arg_string:
		  if (entry->arg_data) *((char **)entry->arg_data) = cfustring_dup_c_str((char *)value);
		  break;
	  default:
		  break;
	}

}

typedef struct {
	int count;
	char **argv;
} _update_extra_ds;

static int
_update_extra(void *data, size_t data_size, void *arg) {
	_update_extra_ds *ds = (_update_extra_ds *)arg;
	data_size = data_size;
	ds->argv[ds->count] = (char *)data;
	ds->count++;

	return 0;
}

extern void
cfuopt_parse(cfuopt_t *context, int *argc, char ***argv, char **error) {
	int i = 0;
	char **args = *argv;
	int is_long_opt = 0;
	int is_short_opt = 0;
	int is_data = 0;
	int is_end_of_opt = 0;
	char *parsed_arg = NULL;
	const char *value = NULL;
	cfuopt_list_entry_t *entry = NULL;
	size_t extra_count = 0;

	error = error;

	if (!context) return;
	if (*argc < 1) return;

	context->progname = cfustring_dup_c_str(args[0]);
	
	if (*argc < 2) return;

	for (i = 1; i < *argc; i++) {
		char *cur_arg = args[i];
		entry = NULL;
		value = NULL;

		if (parsed_arg) free(parsed_arg);
		parsed_arg = NULL;
		
		check_arg(cur_arg, &is_long_opt, &is_short_opt, &is_data, &is_end_of_opt, &parsed_arg,
				  &value);
		if (is_long_opt || is_short_opt) {
			entry = (cfuopt_list_entry_t *)cfuhash_get(context->option_map, parsed_arg);
			if (parsed_arg) free(parsed_arg);
			parsed_arg = NULL;
		}
		else if (is_end_of_opt) {
			if (parsed_arg) free(parsed_arg);
			parsed_arg = NULL;
			i++;
			for (; i < *argc; i++) {
				cfulist_push(context->extra, args[i]);
			}
			break;
		}
		else if (is_data) {
			if (parsed_arg) free(parsed_arg);
			parsed_arg = NULL;
			cfulist_push(context->extra, args[i]);
			continue;
		}

		if (!entry) {
			/* FIXME: return error here if need be */
			continue;
		}

		switch (entry->arg_type) {
		  case cfuopt_arg_bool:
			  _set_entry_val(entry, "1");
			  break;
		  case cfuopt_arg_string:
		  case cfuopt_arg_int:
		  case cfuopt_arg_float:
			  if (value) {
				  if (entry->arg_data) {
					  _set_entry_val(entry, value);
				  }
			  } else {
				  i++;
				  if (i >= *argc) break; /* FIXME: set up error msg here */
				  check_arg(args[i], &is_long_opt, &is_short_opt, &is_data, &is_end_of_opt,
					  &parsed_arg, &value);
				  if (!is_data) {
					  i--;
					  break; /* FIXME: set up error msg here */
				  }
				  _set_entry_val(entry, parsed_arg);
				  free(parsed_arg);
				  parsed_arg = NULL;
			  }
			  break;
		  case cfuopt_arg_string_array:
			  break;
		  case cfuopt_arg_invalid:
			  /* FIXME: really should produce an error msg here */
			  break;
		  default:
			  break;
		}
		
	}

	extra_count = cfulist_num_entries(context->extra);
	*argc = extra_count + 1;
	{
		_update_extra_ds ds;
		size_t update_count = 0;
		ds.count = 1;
		ds.argv = args;
		update_count = cfulist_foreach(context->extra, _update_extra, (void *)&ds);
		assert(update_count + 1 == (unsigned)*argc);
	}
}

static void
_opt_list_free_fn(void *data) {
	cfuopt_list_entry_t *entry = (cfuopt_list_entry_t *)data;
	cfulist_destroy_with_free_fn(entry->param_names, _simple_list_free_fn);
	free(data);
}

extern void
cfuopt_destroy(cfuopt_t *context) {
	if (!context) return;
	cfulist_destroy_with_free_fn(context->option_list, _opt_list_free_fn);
	cfuhash_destroy(context->option_map);
	cfulist_destroy(context->extra);
	free(context->progname);
	free(context);
}

#define ARG_TYPE_TO_STR(type, str) \
	switch(type) { \
	  case cfuopt_arg_invalid: \
		  str = "INVALID"; \
		  break; \
	  case cfuopt_arg_bool: \
		  str = "BOOL"; \
		  break; \
	  case cfuopt_arg_string: \
		  str = "STRING"; \
		  break; \
	  case cfuopt_arg_int: \
		  str = "INT"; \
		  break; \
	  case cfuopt_arg_float: \
		  str = "FLOAT"; \
		  break; \
	  case cfuopt_arg_string_array: \
		  str = "STRING ARRAY"; \
		  break; \
	  default: \
		  str = "INVALID"; \
		  break; \
	}

static int
_cfuopt_pretty_print_foreach(void *key, size_t key_size, void *data, size_t data_size, void *arg) {
	char *name = (char *)key;
	cfuopt_list_entry_t *list_entry = data;
	char *str = NULL;

	key_size = key_size;
	data_size = data_size;
	arg = arg;
	ARG_TYPE_TO_STR(list_entry->arg_type, str);
	printf("%s=%p (%s - %s) => %s, \"%s\"\n", name, (void *)list_entry, (list_entry->required ? "required" : "optional"), str, list_entry->description, list_entry->arg_description);
	
	return 0;
}

extern void
cfuopt_pretty_print(cfuopt_t *context) {
	cfuhash_foreach(context->option_map, _cfuopt_pretty_print_foreach, NULL);
}

static void
_list_simple_free_fn(void *data) {
	free(data);
}

extern void *
_param_map_fn(void *data, size_t data_size, void *arg, size_t *new_data_size) {
	char *name = (char *)data;
	size_t len = 0;
	cfuopt_list_entry_t *opt = (cfuopt_list_entry_t *)arg;

	data_size = data_size;
	if (!name) return NULL;
	len = strlen(name);
	*new_data_size = -1;
	if (len == 0) return cfustring_dup_c_str("");

	if (opt->arg_description && strlen(opt->arg_description) > 0) {
		if (len == 1) {
			return cfustring_sprintf_c_str("-%s %s", name, opt->arg_description);
		}
		else {
			return cfustring_sprintf_c_str("--%s=%s", name, opt->arg_description);
		}
	}
	else {
		if (len == 1) {
			return cfustring_sprintf_c_str("-%s", name);
		}
		else {
			return cfustring_sprintf_c_str("--%s", name);
		}
	}

	return NULL;
}

typedef struct _find_foreach_struct {
	cfulist_t *desc_list;
	size_t max_size;
} _find_foreach_ds;

typedef struct _cfuopt_desc_struct {
	char *desc;
	cfuopt_list_entry_t *entry;
} _cfuopt_desc;

static int
_find_foreach_fn(void *data, size_t data_size, void *arg) {
	cfuopt_list_entry_t *opt = (cfuopt_list_entry_t *)data;
	_find_foreach_ds *ds = (_find_foreach_ds *)arg;
	size_t this_size = 0;
	cfulist_t *param_full_list = NULL;
	char *desc = NULL;
	_cfuopt_desc *desc_ds = CFU_OPT_ALLOC(_cfuopt_desc, 1);

	if (!data) return 0;

	param_full_list = cfulist_map(opt->param_names, _param_map_fn, opt);
	desc = cfulist_join(param_full_list, ", ");

	data_size = data_size;
	
	cfulist_destroy_with_free_fn(param_full_list, _simple_list_free_fn);

	desc_ds->desc = desc;
	desc_ds->entry = opt;
	cfulist_push(ds->desc_list, desc_ds);

	this_size = strlen(desc);
	if (this_size > ds->max_size) ds->max_size = this_size;

	return 0;
}

static size_t
_find_longest_help_desc(cfuopt_t *context, cfulist_t **desc_list) {
	_find_foreach_ds ds;
	ds.max_size = 0;
	ds.desc_list = cfulist_new();

	cfulist_foreach(context->option_list, _find_foreach_fn, (void *)&ds);
	*desc_list = ds.desc_list;
	
	return ds.max_size;
}

typedef struct {
	char *fmt;
	cfulist_t *list;
	cfulist_t *desc_list;
} _help_lines_ds;

static int
_get_help_lines(void *data, size_t data_size, void *arg) {
	_cfuopt_desc *desc_ds = (_cfuopt_desc *)data;
	_help_lines_ds *ds = (_help_lines_ds *)arg;
	char *left_col = NULL;
	const char *desc = NULL;
	char *line = NULL;

	data_size = data_size;

	left_col = desc_ds->desc;
	
	if (!left_col) return 0;

	desc = desc_ds->entry->description;
	if (!desc) desc = "test desc";

	line = cfustring_sprintf_c_str(ds->fmt, left_col, desc);
	cfulist_push(ds->list, (void *)line);

	return 0;
	
}

static void
_desc_list_free(void *data) {
	_cfuopt_desc *desc_ds = (_cfuopt_desc *)data;
	free(desc_ds->desc);
	free(data);
}

extern char *
cfuopt_get_help_str(cfuopt_t *context) {
	cfulist_t *desc_list = NULL;
	size_t max_width = 0;
	char *fmt = NULL;
	_help_lines_ds ds;
	char *help_str = NULL;

	max_width = _find_longest_help_desc(context, &desc_list);
	
	fmt = cfustring_sprintf_c_str(" %%-%us  %%s\n", max_width);
	ds.fmt = fmt;
	ds.list = cfulist_new();
	ds.desc_list = desc_list;

	cfulist_foreach(desc_list, _get_help_lines, (void *)&ds);

	help_str = cfulist_join(ds.list, "\n");

	cfulist_destroy_with_free_fn(ds.list, _list_simple_free_fn);
	cfulist_destroy_with_free_fn(desc_list, _desc_list_free);
	free(fmt);

	return help_str;
}
