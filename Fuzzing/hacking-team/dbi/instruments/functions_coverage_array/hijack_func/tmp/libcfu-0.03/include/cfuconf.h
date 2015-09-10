/* Creation date: 2005-07-08 22:24:03
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

#ifndef _CFU_CONF_H_
#define _CFU_CONF_H_

#include <cfu.h>
#include <cfuhash.h>
#include <cfulist.h>
#include <cfustring.h>

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

	struct cfuconf;
	typedef struct cfuconf cfuconf_t;
	
	/* Apache-style conf files contain directives and containers.
	   Directives are simple one line specifications with or without
	   arguments, e.g.,

	       Doit
		   Expires On
		   LoadModule my_mod modules/my_mod.so

	   Containers have a type and a name associated with them and they
	   in turn contain directives and/or containers, e.g.,

	       <MyContainer test1>
		       Expires Off
			   <DB devdb>
			       DBHost db.example.com
				   DBUser test_user
			   </DB>
		   </MyContainer>

	   Values may be quoted, e.g.

	       DBUser "test user"

	   But must be specified on a single line.  To escape quotes
	   within a quoted string, use the '\' character.

	 */
	

	/* Parse the apache-like conf file specified by file_path,
	 * returning a pointer to a cfuconf_t structure in conf.  Returns
	 * zero on success, less than zero on error.  If an error occurs
	 * and error is not NULL, it will be set to an error message
	 * (which must be free()'d by the caller).
	 */
	extern int cfuconf_parse_file(char *file_path, cfuconf_t **conf, char **error);

	/* Same as cfuconf_parse_file(), except assume the contents of the
	 * file are already in buffer.
	*/
	extern int cfuconf_parse_buffer(char *buffer, cfuconf_t **conf, char **error);

	/* Free all resources used by the cfuconf_t structure */
	extern void cfuconf_destroy(cfuconf_t *conf);

	/* Get a hash of containers at the top level of conf */
	extern cfuhash_table_t * cfuconf_get_containers(cfuconf_t *conf);

	/* Get a hash of directives at the to level */
	extern cfuhash_table_t * cfuconf_get_directives(cfuconf_t *conf);

	/* Get the value of the given directive, assuming there is only one argument */
	extern int cfuconf_get_directive_one_arg(cfuconf_t *conf, char *directive, char **rvalue);

	/* Get the value of the given directive, assuming there are two arguments */
	extern int cfuconf_get_directive_two_args(cfuconf_t *conf, char *directive, char **rvalue,
		char **rvalue2);

	/* Get the value of the given directives, with n arguments */
	extern int cfuconf_get_directive_n_args(cfuconf_t *conf, char *directive, size_t n, ...);

	/* Print out a representation of the parsed configuration */
	extern void cfuconf_pretty_print_conf(cfuconf_t *conf, FILE *fp, size_t indent_level);

#ifdef __cplusplus
}
#endif

#endif
