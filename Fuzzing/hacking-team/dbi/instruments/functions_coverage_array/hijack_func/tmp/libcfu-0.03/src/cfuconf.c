/* Creation date: 2005-07-08 22:23:31
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

#include "cfu.h"
#include "cfuconf.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

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

struct cfuconf {
	libcfu_type type;
	cfuhash_table_t *containers;
	cfuhash_table_t *directives;
	char *container_type;
	char *container_name;
};

typedef struct cfuconf_stack_entry {
	cfuhash_table_t *ht;
	char *container_type;
	char *container_name;
} cfuconf_stack_entry_t;

static cfuconf_t *
cfuconf_new() {
	cfuconf_t *conf = (cfuconf_t *)calloc(1, sizeof(cfuconf_t));
	conf->type = libcfu_t_conf;
	conf->containers = cfuhash_new_with_flags(CFUHASH_IGNORE_CASE);
	conf->directives = cfuhash_new_with_flags(CFUHASH_IGNORE_CASE);
	
	return conf;
}

static void
_free_val_fn(void *data) {
	free(data);
}

static void
_container_free_fn(void *data) {
	assert(cfu_is_conf(data));
	cfuconf_destroy((cfuconf_t *)data);
}

static void
_container_free_hashes_fn(void *data) {
	assert(cfu_is_hash(data));
	cfuhash_destroy_with_free_fn((cfuhash_table_t *)data, _container_free_fn);
}

/*
static void
_container_free_foreach_fn(void *key, size_t key_size, void *data, size_t data_size, void *arg) {
	key = key;
	key_size = key_size;
	arg = arg;
	data_size = data_size;
	assert(cfu_is_hash(data));
	cfuhash_destroy_with_free_fn((cfuhash_table_t *)data, _container_free_fn);
}
*/

static void
_directive_free_val_list_fn(void *data) {
	cfulist_t *val_list = (cfulist_t *)data;
	assert(cfu_is_list(val_list));

	cfulist_destroy_with_free_fn(val_list, _free_val_fn);	
}

static void
_directive_free_fn(void *data) {
	cfulist_t *list = (cfulist_t *)data;
	assert(cfu_is_list(list));

	cfulist_destroy_with_free_fn(list, _directive_free_val_list_fn);
}

extern void
cfuconf_destroy(cfuconf_t *conf) {
	if (conf->containers) {
		/* cfuhash_foreach(conf->containers, _container_free_foreach_fn, NULL); */
		cfuhash_destroy_with_free_fn(conf->containers, _container_free_hashes_fn);
	}
	if (conf->directives) {
		cfuhash_destroy_with_free_fn(conf->directives, _directive_free_fn);
	}
	if (conf->container_type) free(conf->container_type);
	if (conf->container_name) free(conf->container_name);
	free(conf);
}

extern cfuhash_table_t *
cfuconf_get_containers(cfuconf_t *conf) {
	if (!conf) return NULL;
	return conf->containers;
}

extern cfuhash_table_t *
cfuconf_get_directives(cfuconf_t *conf) {
	if (!conf) return NULL;
	return conf->directives;
}

static int
_get_directive_last_val_list(cfuconf_t *conf, char *directive, cfulist_t **val_list) {
	cfuhash_table_t *directives = NULL;
	void *data = NULL;
	size_t data_size = 0;
	cfulist_t *list = NULL;

	if (!conf) {
		return -1;
	}
	
	directives = cfuconf_get_directives(conf);
	if (!directives) {
		return -1;
	}

	if (cfuhash_get_data(directives, (void *)directive, -1, &data, &data_size)) {
		void *val = NULL;
		size_t size = 0;
		list = (cfulist_t *)data;
		if (cfulist_last_data(list, &val, &size)) {
			*val_list = (cfulist_t *)val;
			return 0;
		}
	}
	
	return -1;

}

extern int
cfuconf_get_directive_one_arg(cfuconf_t *conf, char *directive, char **rvalue) {
	cfulist_t *val_list = NULL;
	void *val = NULL;
	size_t size = 0;
	if (_get_directive_last_val_list(conf, directive, &val_list) >= 0) {
		if (cfulist_first_data(val_list, &val, &size)) {
			*rvalue = (char *)val;
			return 0;
		}
	} else {
		return -1;
	}
	
	return -1;
}

extern int
cfuconf_get_directive_two_args(cfuconf_t *conf, char *directive, char **rvalue, char **rvalue2) {
	return cfuconf_get_directive_n_args(conf, directive, 2, rvalue, rvalue2);
}

extern int
cfuconf_get_directive_n_args(cfuconf_t *conf, char *directive, size_t n, ...) {
	va_list ap;
	size_t i = 0;
	char **ptr = NULL;
	int rv = -1;
	cfulist_t *val_list = NULL;
	void *val = NULL;
	size_t size = 0;

	if (_get_directive_last_val_list(conf, directive, &val_list) >= 0) {

		va_start(ap, n);
		for (i = 0; i < n; i++) {
			ptr = va_arg(ap, char **);
			if (cfulist_nth_data(val_list, &val, &size, 1)) {
				*ptr = (char *)val;
			} else {
				i--;
				break;
			}
		}
		va_end(ap);
		
		if (i == n) rv = 0;
		
	}

	return rv;
}

/*
static cfuconf_stack_entry_t *
new_stack_entry(cfuhash_table_t *ht, char *container_type, char *container_name) {
	cfuconf_stack_entry_t *entry =
		(cfuconf_stack_entry_t *)calloc(1, sizeof(cfuconf_stack_entry_t));
	entry->ht = ht;
	entry->container_type = container_type;
	entry->container_name = container_name;
	return entry;
}
*/

static char *
cfuconf_get_line(FILE *fp) {
	char buf[1024];
	cfustring_t *str = cfustring_new_with_initial_size(16);
	char *rv = NULL;

	if (fgets(buf, 1024, fp)) {
		cfustring_append(str, buf);
		while (strlen(buf) == 1024 - 1 && buf[1024 - 2] != '\n') {
			if (!fgets(buf, 1024, fp)) break;
			cfustring_append(str, buf);
		}
		rv = cfustring_get_buffer_copy(str);
	}

	cfustring_destroy(str);
	return rv;
}

static cfulist_t *
_slurp_file_from_fp(FILE *fp) {
	cfulist_t *lines = cfulist_new();
	char *buf = NULL;
	
	while ( (buf = cfuconf_get_line(fp)) ) {
		cfulist_push(lines, buf);
	}

	return lines;
}

static inline int
_is_whitespace(char c) {
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
		return 1;
	}
	return 0;
}

static inline char *
_eat_whitespace(char *buf) {
	char *ptr = buf;

	while (_is_whitespace(*ptr)) {
		ptr++;
	}

	return ptr;
}

static char *
_get_name(char **buf) {
	char *ptr = *buf;
	char *name = *buf;
	name = _eat_whitespace(*buf);
	ptr = name;

	if (!*ptr || *ptr == '>') {
		*buf = ptr;
		return NULL;
	}
	while (*ptr && *ptr != '>' && !_is_whitespace(*ptr)) ptr++;
	name = cfustring_dup_c_str_n(name, ptr - name);
	*buf = ptr;
	
	return name;
}

static char *
_get_value(char **buf) {
	char *ptr = *buf;
	char *value = *buf;
	value = _eat_whitespace(*buf);
	ptr = value;

	if (!*ptr || *ptr == '>') {
		*buf = ptr;
		return NULL;
	}
	while (*ptr && *ptr != '>' && !_is_whitespace(*ptr)) ptr++;
	value = cfustring_dup_c_str_n(value, ptr - value);
	*buf = ptr;
	
	return value;
}

static char *
_dup_c_str_n_drop_escape(const char *str, size_t n, char escape) {
	size_t len = n;
	char *ns = NULL;
	char *ptr = NULL;
	char *ns_ptr = NULL;
	char last_char = '\000';
	char *end = (char *)str + n;
	
	if (n == 0) return NULL;

	ptr = (char *)str;
	ns_ptr = ns = (char *)calloc(len + 1, 1);

	last_char = *ptr;
	for (ptr = (char *)str; ptr < end; ptr++) {
		if (*ptr == escape && (last_char != escape || ptr == (char *)str)) {
			if (ptr == (char *)str) {
				if (*(ptr + 1) == escape) {
					last_char = *ptr;
					*ns_ptr = *ptr;
					ns_ptr++;
				}
			}
			last_char = *ptr;
		} else {
			last_char = *ptr;
			*ns_ptr = *ptr;
			ns_ptr++;
		}
	}
	*ns_ptr = '\000';

	return ns;
}

static char *
_get_quoted_value(char **buf, char quote) {
	char *ptr = NULL;
	char *value = NULL;
	char last_char = '\000';
	int done = 0;
	int found_escape = 0;

	if (!buf) return NULL;
	if (!*buf) return NULL;
	ptr = value = *buf;
	last_char = *ptr;
	while (!done) {
		while (*ptr && *ptr != quote) {
			if (*ptr == '\\') found_escape = 1;
			last_char = *ptr;
			ptr++;
		}
		if (last_char != '\\' || !*ptr) done = 1;
		else {
			if (last_char == '\\') {
				ptr++;
				found_escape = 1;
			}
		}
	}
	if (!*ptr) return NULL;
	*buf = ptr;
	
	if (found_escape) {
		value = _dup_c_str_n_drop_escape(value, ptr - value, '\\');
	} else {
		value = cfustring_dup_c_str_n(value, ptr - value);
	}
	return value;
}

static char *
_get_next_value(char **buf) {
	char *ptr = *buf;
	char *value = *buf;
	value = _eat_whitespace(*buf);
	ptr = value;

	if (!*ptr || *ptr == '>') {
		*buf = ptr;
		return NULL;
	}
	if (*ptr == '"' || *ptr == '\'') {
		char quote = *ptr;
		ptr++;
		value = _get_quoted_value(&ptr, quote);
	} else {
		while (*ptr && *ptr != '>' && !_is_whitespace(*ptr)) ptr++;
		value = cfustring_dup_c_str_n(value, ptr - value);
	}

	*buf = ptr;
	
	return value;
}

#if 0
static char *
_get_long_value(char **buf) {
	char *ptr = *buf;
	char *value = *buf;
	size_t len = strlen(*buf);

	value = _eat_whitespace(*buf);
	ptr = *buf + len;
	while (ptr > value && _is_whitespace(*ptr)) {
		ptr--;
	}

	if (ptr == value) return NULL;

	value = cfustring_dup_c_str_n(value, ptr - value - 1);
	*buf = ptr;
	
	return value;
}
#endif

/* FIXME: implement these */
/* get directives */
/* get_containers */
/* get_containers_of_type */
/* xpath type access */

static cfuconf_t *
_cfuconf_parse_list(cfulist_t *lines, char **error) {
	char *line = NULL;
	char *ptr = NULL;
	cfulist_t *stack = cfulist_new();
	size_t cur_line = 0;
	cfuconf_t *conf = cfuconf_new();
	cfuconf_t *cur_conf = NULL;

	cur_conf = conf;

	while ( (line = (char *)cfulist_shift(lines)) ) {
		cur_line++;
		ptr = _eat_whitespace(line);
		if (!*ptr || *ptr == '#')  {
			/* blank line or comment */
			free(line);
			continue;
		}
		
		if (*ptr == '<') { /* opening or closing container tag */
			ptr++;
			if (*ptr == '/') {
				/* closing tag */
				char *name = NULL;

				ptr++;
				name = _get_name(&ptr);
				if (!name) {
					if (error) {
						*error = cfustring_sprintf_c_str("cfuconf: syntax error at line %u\n",
							cur_line);
					}
					free(line);
					free(name);
					return NULL;
				}

				if (strcasecmp(name, cur_conf->container_type)) {
					if (error) {
						*error = cfustring_sprintf_c_str("Error: syntax error at line %u: "
							"unmatched container should be %s\n", cur_line,
							cur_conf->container_type);
					}
					/* FIXME: clean up memory here */
					free(line);
					free(name);
					return NULL;
				}

				free(name); name = NULL;

				cur_conf = cfulist_pop(stack);
				if (!cur_conf) {
					free(line);
					return NULL;
				}

				/* fprintf(stderr, "====> got closing tag '%s'\n", name); */
			} else {
				/* opening tag */
				char *name = NULL;
				char *value = NULL;
				cfuhash_table_t *this_hash = NULL;
				cfuconf_t *new_conf = NULL;
				name = _get_name(&ptr);
				if (!name) {
					if (error) {
						*error = cfustring_sprintf_c_str("cfuconf: syntax error at line %u\n",
							cur_line);
					}
					free(line);

					/* FIXME: clean up memory here */
					return NULL;
				}

				ptr = _eat_whitespace(ptr);
				value = _get_value(&ptr);

				assert(cur_conf);

				if ( !(this_hash = cfuhash_get(cur_conf->containers, name)) ) {
					this_hash = cfuhash_new_with_flags(CFUHASH_IGNORE_CASE);
					cfuhash_put(cur_conf->containers, name, (void *)this_hash);
				}
				
				if ( !(new_conf = cfuhash_get(this_hash, value)) ) {
					new_conf = cfuconf_new();
					new_conf->container_type = name;
					new_conf->container_name = value;
					cfuhash_put(this_hash, value, (void *)new_conf);
				} else {
					free(name); name = NULL;
					free(value); value = NULL;
				}
				
				cfulist_push(stack, (void *)cur_conf);
				cur_conf = new_conf;

				/*
				fprintf(stderr, "====> got opening tag '%s' with param '%s'\n", name, value);
				*/
			}
		} else {
			char *name = NULL;
			char *value = NULL;
			cfulist_t *list = NULL;
			cfulist_t *val_list = NULL;

			name = _get_name(&ptr);
			ptr = _eat_whitespace(ptr);

			if ( !(list = cfuhash_get(cur_conf->directives, name)) ) {
				list = cfulist_new();
				cfuhash_put(cur_conf->directives, name, (void *)list);
			}
			free(name); name = NULL;
			
			val_list = cfulist_new();
			cfulist_enqueue(list, (void *)val_list);
			
			/* fprintf(stderr, "====> got '%s' =>", name); */
			while ( (value = _get_next_value(&ptr) ) ) {
				/* fprintf(stderr, " '%s'", value); */
				cfulist_enqueue(val_list, (void *)value);
				if (*ptr) {
					ptr++;
					ptr = _eat_whitespace(ptr);
				}
			}
			/* fprintf(stderr, "\n"); */
		}

		free(line);
	}

	cfulist_destroy(stack);
	return conf;
}

extern int
cfuconf_parse_file(char *file_path, cfuconf_t **ret_conf, char **error) {
	FILE *fp = NULL;
	cfulist_t *lines = NULL;
	cfuconf_t *conf = NULL;

	if (! (fp = fopen(file_path, "r")) ) {
		*ret_conf = NULL;
		if (error) {
			*error = cfustring_sprintf_c_str("Couldn't open file");
		}
		return -1;
	}

	lines = _slurp_file_from_fp(fp);
	fclose(fp); fp = NULL;
	if (!lines) return -1;
	
	conf = _cfuconf_parse_list(lines, error);
	cfulist_destroy(lines);
	*ret_conf = conf;
	if (conf) return 0;
	return -1;
}

extern int
cfuconf_parse_buffer(char *buffer, cfuconf_t **ret_conf, char **error) {
	cfulist_t *lines = cfulist_new();
	char **strings = NULL;
	size_t num_strings = 0;
	cfuconf_t *conf = NULL;
	size_t i = 0;

	if (!buffer) return -1;

	strings = cfustring_c_str_split(buffer, &num_strings, 0, "\r\n", "\n", NULL);
	if (!strings) return -1;
	
	for (i = 0; i < num_strings; i++) {
		cfulist_push_string(lines, strings[i]);
	}
	
	conf = _cfuconf_parse_list(lines, error);
	cfulist_destroy(lines);
	*ret_conf = conf;

	free(strings);

	if (conf) return 0;
	return -1;	
}



static void
print_indent(size_t depth, FILE *fp) {
	size_t i = 0;
	if (depth <= 0) return;
	for (i = 0; i < depth; i++) {
		fprintf(fp, "\t");
	}
}

static void
print_container(cfuhash_table_t *conf_hash, size_t depth) {
	char **keys = NULL;
	size_t num_keys = 0;
	void *val = NULL;
	cfulist_t *ol = NULL;
	cfulist_t *il = NULL;
	size_t i = 0;
	size_t data_size = 0;
	void *entry = NULL;
	cfuhash_table_t *ht = NULL;
	keys = (char **)cfuhash_keys(conf_hash, &num_keys, 0);
	for (i = 0; i < num_keys; i++) {
		entry = cfuhash_get(conf_hash, keys[i]);
		if (cfu_is_list(entry)) {
			ol = (cfulist_t *)entry;
			cfulist_reset_each(ol);
			while (cfulist_next_data(ol, (void *)&il, &data_size)) {
				print_indent(depth, stderr);
				fprintf(stderr, "%s =>", keys[i]);
				cfulist_reset_each(il);
				while (cfulist_next_data(il, &val, &data_size)) {
					fprintf(stderr, " '%s'", (char *)val);
				}
				fprintf(stderr, "\n");
			}
		} else if (cfu_is_hash(entry)) {
			/* container */
			char **container_names = NULL;
			size_t num_containers = 0;
			char *container_type = keys[i];
			char *container_name = NULL;
			cfuhash_table_t *container = NULL;
			size_t j = 0;
			ht = (cfuhash_table_t *)entry;
			container_names = (char **)cfuhash_keys(ht, &num_containers, 0);
			for (j = 0; j < num_containers; j++) {
				container_name = container_names[j];

				print_indent(depth, stderr);
				fprintf(stderr, "%s: '%s'\n", container_type, container_name);

				container = cfuhash_get(ht, container_name);
				print_container(container, depth + 1);
			}
			
			free(container_names);
		}
		free(keys[i]);
	}
	free(keys);
}

typedef struct directive_foreach_ds {
	size_t depth;
	FILE *fp;
	char *name;
} directive_foreach_ds;

static void print_conf(cfuconf_t *conf, size_t depth, FILE *fp);

static int
print_sub_container_foreach_fn(void *name, size_t key_size, void *data, size_t data_size,
	void *arg) {
	directive_foreach_ds *ds = (directive_foreach_ds *)arg;

	name = name;
	key_size = key_size;
	data_size = data_size;

	print_indent(ds->depth, ds->fp);
	fprintf(ds->fp, "container %s '%s'\n", ds->name, (char *)name);
	print_conf((cfuconf_t *)data, ds->depth + 1, ds->fp);
	return 0;
}

static int
print_container_foreach_fn(void *name, size_t key_size, void *data, size_t data_size, void *arg) {
	directive_foreach_ds *ds = (directive_foreach_ds *)arg;
	directive_foreach_ds *new_ds = (directive_foreach_ds *)calloc(1, sizeof(directive_foreach_ds));

	memcpy(new_ds, ds, sizeof(directive_foreach_ds));
	new_ds->name = (char *)name;

	key_size = key_size;
	data_size = data_size;

	cfuhash_foreach((cfuhash_table_t *)data, print_sub_container_foreach_fn, new_ds);
	free(new_ds);
	return 0;
}


static int
print_directive_list_foreach_fn(void *data, size_t data_size, void *arg) {
	cfulist_t *val_list = (cfulist_t *)data;
	directive_foreach_ds *ds = (directive_foreach_ds *)arg;
	char *str = NULL;
	
	data_size = data_size;
	if (!val_list) return 0;
	assert(cfu_is_list(val_list));
	str = cfulist_join(val_list, ", ");
	print_indent(ds->depth, ds->fp);
	fprintf(ds->fp, "directive '%s' => %s\n", ds->name, str);
	free(str);
	return 0;
}

static int
print_conf_foreach_directive(void *name, size_t key_size, void *data, size_t data_size, void *arg) {
	directive_foreach_ds *ds = (directive_foreach_ds *)arg;
	cfulist_t *this_directive_list = (cfulist_t *)data;
	directive_foreach_ds *new_ds = (directive_foreach_ds *)calloc(1, sizeof(directive_foreach_ds));

	key_size = key_size;
	data_size = data_size;
	
	new_ds->fp = ds->fp;
	new_ds->depth = ds->depth;
	new_ds->name = name;
	
	assert(cfu_is_list(this_directive_list));
	cfulist_foreach(this_directive_list, print_directive_list_foreach_fn, new_ds);
	
	free(new_ds);
	return 0;
}

static void
print_conf(cfuconf_t *conf, size_t depth, FILE *fp) {
	directive_foreach_ds *ds = (directive_foreach_ds *)calloc(1, sizeof(directive_foreach_ds));
	ds->depth = depth;
	ds->fp = fp;

	cfuhash_foreach(cfuconf_get_directives(conf), print_conf_foreach_directive, ds);
	cfuhash_foreach(cfuconf_get_containers(conf), print_container_foreach_fn, ds);

	free(ds);
}

extern void
cfuconf_pretty_print_conf(cfuconf_t *conf, FILE *fp, size_t indent_level) {
	print_conf(conf, indent_level, fp);
}
