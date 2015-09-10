/* Creation date: 2005-06-26 19:59:10
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

#include "cfustring.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

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

struct cfustring {
	libcfu_type type;
	size_t max_size;
	size_t used_size;
	char *str;
};

extern cfustring_t *
cfustring_new() {
	return cfustring_new_with_initial_size(0);
}

extern cfustring_t *
cfustring_new_with_initial_size(size_t initial_size) {
	cfustring_t *cfu_str = (cfustring_t *)calloc(1, sizeof(cfustring_t));
	cfu_str->type = libcfu_t_string;
	if (initial_size > 0) {
		cfu_str->str = (char *)calloc(initial_size, 1);
		cfu_str->max_size = initial_size;
		cfu_str->used_size = 1;
	}
	return cfu_str;
}

extern cfustring_t *
cfustring_new_from_string(const char *string) {
	cfustring_t *cfu_str = cfustring_new();
	cfustring_append(cfu_str, string);
	return cfu_str;
}

extern int
cfustring_dup(cfustring_t *cfu_str, const char *string) {
	if (!string) {
		cfu_str->max_size = 0;
		cfu_str->used_size = 0;
		free(cfu_str->str);
		cfu_str->str = NULL;
		return 1;
	}

	cfustring_clear(cfu_str);
	cfustring_append(cfu_str, string);
	return 1;
}

extern int
cfustring_clear(cfustring_t *cfu_str) {
	if (cfu_str->str) {
		cfu_str->str[0] = '\000';
		cfu_str->used_size = 1;
	}
	return 1;
}

extern int
cfustring_append_n(cfustring_t *cfu_str, const char *string, size_t n) {
	size_t str_len = 0;
	if (!string) return 1;

	if (n) {
		size_t i = 0;
		const char *ptr = string;
		for (i = 0; i < n; i++) {
			if (*ptr) {
				str_len++;
			} else {
				break;
			}
		}
	} else {
		str_len = strlen(string);
	}

	if (!cfu_str->str) {
		cfu_str->str = (char *)malloc(str_len + 1);
		cfu_str->max_size = str_len + 1;
		cfu_str->used_size = 1;
		cfu_str->str[0] = '\000';
	}

	if (cfu_str->used_size + str_len + 1 > cfu_str->max_size) {
		/* allocate more memory and copy over */
		char *tmp = NULL;

		if (cfu_str->max_size * 2 >= cfu_str->used_size + str_len + 1) {
			cfu_str->max_size *= 2;
		} else {
			cfu_str->max_size = cfu_str->used_size + str_len + 1;
		}
		tmp = (char *)malloc(cfu_str->max_size);
		memcpy(tmp, cfu_str->str, cfu_str->used_size);
		free(cfu_str->str);
		cfu_str->str = tmp;
	}

	memcpy(&cfu_str->str[cfu_str->used_size - 1], string, str_len);
	/* strcat(cfu_str->str, string); */
	cfu_str->used_size += str_len;
	cfu_str->str[cfu_str->used_size - 1] = '\000';


	return 1;
}

extern int
cfustring_append(cfustring_t *cfu_str, const char *string) {
	return cfustring_append_n(cfu_str, string, 0);
}

extern char *
cfustring_get_buffer(cfustring_t *cfu_str) {
	return cfu_str->str;
}

extern char *
cfustring_get_buffer_copy(cfustring_t *cfu_str) {
	char *buffer = NULL;
	if (!cfu_str->str) return NULL;
	
	buffer = (char *)calloc(cfu_str->used_size, 1);
	memcpy(buffer, cfu_str->str, cfu_str->used_size);
	return buffer;
}

static char *
_dup_str(const char *str) {
	size_t len = strlen(str) + 1;
	char *ns = (char *)calloc(len, 1);
	memcpy(ns, str, len);
	return ns;
}

static char *
_dup_str_n(const char *str, size_t n) {
	size_t len = n;
	char *ns;

	if (n == 0) return NULL;

	ns = (char *)calloc(len + 1, 1);
	memcpy(ns, str, len);
	ns[len] = '\000';
	return ns;
}

extern char *
cfustring_dup_c_str(const char *str) {
	return _dup_str(str);
}

extern char *
cfustring_dup_c_str_n(const char *str, size_t n) {
	return _dup_str_n(str, n);
}

static char *
_check_sep(char **start, char **seps, size_t num_seps, char **sep_chk_ptrs, int last) {
	size_t i;
	char *end = *start;

	for (i = 0; i < num_seps; i++) sep_chk_ptrs[i] = seps[i];

	if (!*start || !*(*start)) return NULL;

	if (last) {
		for(; *end; end++);
		if (end != *start) {
			char *rv = _dup_str_n(*start, end - *start);
			*start = end;
			return rv;
		}
		return NULL;
	}
		
	for (end = *start; *end; end++) {
		for (i = 0; i < num_seps; i++) {
			if (*sep_chk_ptrs[i] == *end) {
				sep_chk_ptrs[i]++;
				if (!*sep_chk_ptrs[i]) {
					/* got a match */
					size_t size = end - *start + 1 - (sep_chk_ptrs[i] - seps[i]);
					char *rv = _dup_str_n(*start, size);
					*start = end + 1;
					return rv;
				}
			} else {
				sep_chk_ptrs[i] = seps[i];
			}
		}
	}

	if (end != *start) {
		char *rv = _dup_str_n(*start, end - *start);
		*start = end;
		return rv;
	}

	return NULL;
}

static char **
__cfustring_split_to_raw(cfustring_t *cfu_str, size_t *num_strings, size_t num_seps, size_t limit,
	va_list ap) {
	char *sep = NULL;
	size_t i = 0;
	char **sep_array = (char **)calloc(num_seps, sizeof(char *));
	char **sep_chk_ptrs = (char **)calloc(num_seps, sizeof(char *));
	char **ret_strings = (char **)calloc(2, sizeof(char *));
	u_int max_ret_strings = 2;
	u_int used_ret_strings = 0;
	char *start = NULL;
	char *end = NULL;
	char *next_str = NULL;
	int last = 0;

	*num_strings = 0;

	if (limit == 1) {
		ret_strings[0] = cfustring_get_buffer_copy(cfu_str);
		*num_strings = 1;
		return ret_strings;
	}

	for (i = 0; i < num_seps; i++) {
		sep = va_arg(ap, char *);
		sep_array[i] = _dup_str(sep);
	}

	start = end = cfustring_get_buffer(cfu_str);
	if (!end) {
		*num_strings = 0;
		free(ret_strings);
		for (i = 0; i < num_seps; i++) {
			free(sep_array[i]);
		}
		free(sep_array);
		return NULL;
	}
	
	while ( (next_str = _check_sep(&end, sep_array, num_seps, sep_chk_ptrs, last)) ) {
		if (used_ret_strings == max_ret_strings) {
			/* allocate more space */
			size_t new_size = max_ret_strings << 1;
			char **tmp = (char **)calloc(new_size, sizeof(char *));
			for (i = 0; i < used_ret_strings; i++) tmp[i] = ret_strings[i];
			free(ret_strings);
			ret_strings = tmp;
			max_ret_strings = new_size;
		}
		ret_strings[used_ret_strings] = next_str;
		used_ret_strings++;

		if (limit > 0 && used_ret_strings == limit - 1) {
			last = 1;
		}
	}

	for (i = 0; i < num_seps; i++) {
		free(sep_array[i]);
	}
	free(sep_array);
	free(sep_chk_ptrs);

	*num_strings = used_ret_strings;
	return ret_strings;
}

extern cfustring_t **
cfustring_split(cfustring_t *cfu_str, size_t *num_strings, size_t limit, ...) {
	va_list ap;
	char **strings = NULL;
	char *sep = NULL;
	cfustring_t **rv = NULL;
	size_t i = 0;
	size_t num_seps = 0;

	va_start(ap, limit);
	sep = va_arg(ap, char *);
	while (sep) {
		num_seps++;
		sep = va_arg(ap, char *);
	}
	va_end(ap);

	va_start(ap, limit);
	strings = __cfustring_split_to_raw(cfu_str, num_strings, num_seps, limit, ap);
	va_end(ap);

	if (!*num_strings) return NULL;
	rv = (cfustring_t **)malloc(*num_strings * sizeof(cfustring_t *));
	for (i = 0; i < *num_strings; i++) {
		rv[i] = cfustring_new_from_string(strings[i]);
		free(strings[i]);
	}
	free(strings);

	return rv;
}


extern char **
cfustring_split_to_c_str(cfustring_t *cfu_str, size_t *num_strings, size_t limit, ...) {
	char **rv = NULL;
	va_list ap;
	size_t num_seps = 0;
	char *sep = NULL;

	va_start(ap, limit);
	sep = va_arg(ap, char *);
	while (sep) {
		num_seps++;
		sep = va_arg(ap, char *);
	}
	va_end(ap);

	va_start(ap, limit);
	rv = __cfustring_split_to_raw(cfu_str, num_strings, num_seps, limit, ap);
	va_end(ap);

	return rv;
}

extern char *
cfustring_sprintf_c_str(const char *fmt, ...) {
	va_list ap;
	char *str = NULL;
	cfustring_t *cfu_str = cfustring_new();
	va_start(ap, fmt);
	cfustring_vsprintf(cfu_str, fmt, ap);	
	va_end(ap);

	str = cfustring_get_buffer_copy(cfu_str);
	cfustring_destroy(cfu_str);

	return str;
}

extern size_t
cfustring_sprintf(cfustring_t *cfu_str, const char *fmt, ...) {
	va_list ap;
	size_t rv = 0;
	va_start(ap, fmt);
	rv = cfustring_vsprintf(cfu_str, fmt, ap);
	va_end(ap);

	return rv;
}

static int
_safe_sprintf(char **buf, size_t *buf_size, const char *fmt, ...) {
	va_list ap;
	int rv = 0;
	int done = 0;

	if (!(*buf) || *buf_size == 0) {
		*buf_size = 128;
		if (*buf) free(*buf);
		*buf = (char *)malloc(*buf_size);
	}

	while (!done) {
		va_start(ap, fmt);
		rv = vsnprintf(*buf, *buf_size, fmt, ap);
		va_end(ap);
		if (rv >= (int)(*buf_size) - 1) {
			*buf_size *= 2;
			free(*buf);
			*buf = (char *)malloc(*buf_size);
		}
		else {
			done = 1;
		}
	}

	return rv;
}

static char *
_safe_strncpy(char **buf, size_t *buf_size, const char *src, size_t size) {
	char *rv = NULL;

	if (!(*buf) || *buf_size == 0) {
		*buf_size = size + 1;
		if (*buf) free(*buf);
		*buf = (char *)malloc(*buf_size);
		(*buf)[0] = '\000';
	}

	if (size > *buf_size - 1) {
		*buf_size = size + 1;
		if (*buf) free(*buf);
		*buf = (char *)malloc(*buf_size);
		(*buf)[0] = '\000';
	}

	rv = strncpy(*buf, src, size);

	return rv;
}

extern size_t
cfustring_vsprintf(cfustring_t *cfu_str, const char *fmt_in, va_list ap) {
	const char *ptr = NULL;
	const char *start = NULL;
	const char *end = NULL;
	size_t byte_count = 0;
	size_t buf_size = 128;
	size_t buf2_size = 128;
	char *buf = NULL;
	char *buf2 = NULL;
	const char *fmt = NULL;

	buf = (char *)malloc(buf_size);
	buf2 = (char *)malloc(buf2_size);

	cfustring_clear(cfu_str);

	ptr = start = end = fmt = fmt_in;
	while (*fmt) {
		if (*fmt == '%') {
			ptr = fmt;
			fmt++;
			if (*fmt == '%') {
				end++;
				cfustring_append_n(cfu_str, start, ptr - start + 1);
				byte_count += ptr - start + 1;

				fmt++;
				start = end = fmt;
				continue;
			}

			while (*fmt && *fmt != 'u' && *fmt != 's' && *fmt != 'd' && *fmt != 'f' && *fmt != 'F'
				&& *fmt != 'x' && *fmt != 'X' && *fmt != 'p'
				&& *fmt != ' ' && *fmt != '\t') fmt++;

			if (*fmt == 'u' || *fmt == 'd' || *fmt == 'f' || *fmt == 'F'
				|| *fmt == 'x' || *fmt == 'X' || *fmt == 'p') {
				unsigned num = 0;
				double f = 0;
				int d = 0;
				void *p = NULL;
				size_t size = fmt - ptr + 1;

				_safe_strncpy(&buf2, &buf2_size, ptr, size);
				buf2[size] = '\000';

				switch (*fmt) {
				  case 'u':
				  case 'x':
				  case 'X':
				  case 'o':
					  num = va_arg(ap, unsigned);
					  _safe_sprintf(&buf, &buf_size, buf2, num);
					  break;
				  case 'd':
				  case 'c':
					  d = va_arg(ap, int);
					  _safe_sprintf(&buf, &buf_size, buf2, d);
					  break;
				  case 'f':
				  case 'F':
				  case 'e':
				  case 'E':
				  case 'g':
				  case 'G':
				  case 'a':
				  case 'A':
					  f = va_arg(ap, double);
					  _safe_sprintf(&buf, &buf_size, buf2, f);
					  break;
				  case 'p':
					  p = va_arg(ap, void *);
					  _safe_sprintf(&buf, &buf_size, buf2, p);
					  break;
				  default:
					  break;
				}

				cfustring_append_n(cfu_str, start, end - start + 1);
				byte_count += end - start + 1;
				cfustring_append(cfu_str, buf);
				byte_count += strlen(buf);
				fmt++;
				start = end = fmt;
				continue;
			} else if (*fmt == 's') {
				char *s = NULL;
				size_t size = fmt - ptr + 1;
				s = va_arg(ap, char *);

				buf[0] = '\000';

				_safe_strncpy(&buf2, &buf2_size, ptr, size);
				buf2[size] = '\000';

				_safe_sprintf(&buf, &buf_size, buf2, s);

				if (ptr != start) {
					cfustring_append_n(cfu_str, start, end - start + 1);
					byte_count += end - start + 1;
				}
				if (s) {
					cfustring_append(cfu_str, buf);
					byte_count += strlen(buf);
				} else {
					s = "(null)";
					cfustring_append(cfu_str, s);
					byte_count += strlen(s);
				}
				fmt++;
				start = end = fmt;
				continue;
			} else if (*fmt == ' ' || *fmt == '\t') {
				cfustring_append_n(cfu_str, start, end - start + 1);
				byte_count += end - start + 1;
				fmt++;
				start = end = fmt;
				continue;
			}
			
		} else {
			byte_count++;
			end = fmt;
		}
		fmt++;

	}

	if (end != start) {
		cfustring_append_n(cfu_str, start, end - start + 1);
		byte_count += end - start + 1;
	}

	free(buf);
	free(buf2);

	return byte_count;
}

extern int
cfustring_destroy(cfustring_t *cfu_str) {
	free(cfu_str->str);
	free(cfu_str);
	return 1;
}

extern char **
cfustring_c_str_split(const char *c_str, size_t *num_strings, size_t limit, ...) {
	cfustring_t *cfu_str = cfustring_new_from_string(c_str);
	char **rv = NULL;
	va_list ap;
	size_t num_seps = 0;
	char *sep = NULL;

	va_start(ap, limit);
	sep = va_arg(ap, char *);
	while (sep) {
		num_seps++;
		sep = va_arg(ap, char *);
	}
	va_end(ap);

	va_start(ap, limit);
	rv = __cfustring_split_to_raw(cfu_str, num_strings, num_seps, limit, ap);
	va_end(ap);

	cfustring_destroy(cfu_str);

	return rv;	
}
