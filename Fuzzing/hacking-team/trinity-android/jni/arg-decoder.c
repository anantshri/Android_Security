/*
 * Routines to take a syscallrecord and turn it into an ascii representation.
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include "arch.h"	//PAGE_MASK
#include "log.h"
#include "params.h"	// logging, monochrome, quiet_level
#include "pids.h"
#include "shm.h"
#include "syscall.h"
#include "tables.h"
#include "utils.h"
#include "base64.h"
#include "time.h"
#include "struct_analyze.h"

#define MY_ENCODING "ISO-8859-1"

xmlTextWriterPtr writer = NULL;

static char * decode_argtype(char *sptr, unsigned long reg, enum argtype type, const char *arg_name, struct misc_arg_info *arg_misc)
{
        size_t s = 0;
	char *enc_buf = NULL;
	char asd[128];
	char arg_token[56];

	memset(arg_token, 0, sizeof(arg_token));

	// Create a new ARG attribute
	xmlTextWriterStartElement(writer, "ARG");
	xmlTextWriterWriteAttribute(writer, "name", arg_name);

	switch (type) {
	case ARG_PATHNAME:
	  xmlTextWriterWriteElement(writer, BAD_CAST "ARG_PATHNAME", (char *) reg);
	  sptr += sprintf(sptr, "\"%s\"", (char *) reg);
	  break;

	case ARG_PID:
	  strncpy(arg_token, "ARG_PID", sizeof(arg_token));
	  
	case ARG_FD:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_FD", sizeof(arg_token));

	  snprintf(asd, sizeof(asd), "%lu", (long) reg);
	  xmlTextWriterWriteFormatElement(writer, arg_token, "%s", asd);
	  sptr += sprintf(sptr, "%s%ld", ANSI_RESET, (long) reg);
	  break;

	case ARG_MODE_T:
	  xmlTextWriterWriteFormatElement(writer, BAD_CAST "ARG_MODE_T", "%d", (int) reg);
	  sptr += sprintf(sptr, "%s%o", ANSI_RESET, (mode_t) reg);
	  break;

	case ARG_ADDRESS:
	  strncpy(arg_token, "ARG_ADDRESS", sizeof(arg_token));

	case ARG_NON_NULL_ADDRESS:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_NON_NULL_ADDRESS", sizeof(arg_token));
	  
	case ARG_IOVEC:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_IOVEC", sizeof(arg_token));

	case ARG_SOCKADDR:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_SOCKADDR", sizeof(arg_token));

	  xmlTextWriterWriteFormatElement(writer, arg_token, "0x%lx", reg);
	  struct_recursive_analyze( (void *) reg, arg_misc->struct_type, writer);

	  sptr += sprintf(sptr, "0x%lx", reg);
	  break;

	case ARG_MMAP:
	  /* Although generic sanitise has set this to a map struct,
	   * common_set_mmap_ptr_len() will subsequently set it to the ->ptr
	   * in the per syscall ->sanitise routine. */
	  xmlTextWriterWriteFormatElement(writer, BAD_CAST "ARG_MMAP", "%p", (void *) reg);

	  /*if(len) {
	    s = base64_size(len);
	    enc_buf = zmalloc(s+1);
	    base64_encode_binary(enc_buf, (unsigned char *) reg, len);
	    }*/
		 
	  sptr += sprintf(sptr, "%p", (void *) reg);
	
	  /* xmlTextWriterWriteFormatElement(writer, BAD_CAST "Pointer", "%p", (void *) reg);
	     xmlTextWriterWriteElement(writer, BAD_CAST "Blob", enc_buf);
	     xmlTextWriterEndElement(writer); */
	  
	  //if(enc_buf != NULL)
	  //free(enc_buf);
	  
	  break;

	case ARG_OP:
	  strncpy(arg_token, "ARG_OP", sizeof(arg_token));

	case ARG_LIST:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_LIST", sizeof(arg_token));
	  xmlTextWriterWriteFormatElement(writer, arg_token, "0x%lx", reg);
	  
	  sptr += sprintf(sptr, "0x%lx", reg);
	  break;

	case ARG_UNDEFINED:
	  strncpy(arg_token, "ARG_UNDEFINED", sizeof(arg_token));
	  
	case ARG_LEN:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_LEN", sizeof(arg_token));
	  
	case ARG_RANGE:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_RANGE", sizeof(arg_token));

	case ARG_CPU:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_CPU", sizeof(arg_token));

	case ARG_IOVECLEN:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_IOVECLEN", sizeof(arg_token));

	case ARG_SOCKADDRLEN:
	  if(strlen(arg_token) == 0)
	    strncpy(arg_token, "ARG_SOCKADDRLEN", sizeof(arg_token));
	  
	  if (((long) reg < -16384) || ((long) reg > 16384)) {
	    xmlTextWriterWriteFormatElement(writer, arg_token, "0x%lx", reg);
	    /* Print everything outside -16384 and 16384 as hex. */
	    sptr += sprintf(sptr, "0x%lx", reg);
	  } else {
	    xmlTextWriterWriteFormatElement(writer, arg_token, "0x%ld", (long) reg);
	    /* Print everything else as signed decimal. */
	    sptr += sprintf(sptr, "%ld", (long) reg);
	  }
	  sptr += sprintf(sptr, "%s", ANSI_RESET);
	  break;
	}
	
	xmlTextWriterEndElement(writer);
	return sptr;
}


static char * render_arg(struct syscallrecord *rec, char *sptr, unsigned int argnum, struct syscallentry *entry)
{
	const char *name = NULL;
	unsigned long reg = 0;
	enum argtype type = 0;
	struct misc_arg_info *arg_misc;

	switch (argnum) {
	case 1:	type = entry->arg1type;
		name = entry->arg1name;
		arg_misc = &entry->arg1misc;
		reg = rec->a1;
		break;

	case 2:	type = entry->arg2type;
		name = entry->arg2name;
		arg_misc = &entry->arg2misc;
		reg = rec->a2;
		break;

	case 3:	type = entry->arg3type;
		name = entry->arg3name;
		arg_misc = &entry->arg3misc;
		reg = rec->a3;
		break;

	case 4:	type = entry->arg4type;
		name = entry->arg4name;
		arg_misc = &entry->arg4misc;
		reg = rec->a4;
		break;

	case 5:	type = entry->arg5type;
		name = entry->arg5name;
		arg_misc = &entry->arg5misc;
		reg = rec->a5;
		break;

	case 6:	type = entry->arg6type;
		name = entry->arg6name;
		arg_misc = &entry->arg6misc;
		reg = rec->a6;
		break;
	}

	if (argnum != 1)
		sptr += sprintf(sptr, "%s, ", ANSI_RESET);

	sptr += sprintf(sptr, "%s=", name);

	sptr = decode_argtype(sptr, reg, type, name, arg_misc);

	if (entry->decode != NULL) {
		char *str;

		str = entry->decode(rec, argnum);
		if (str != NULL) {
			sptr += sprintf(sptr, "%s", str);
			free(str);
		}
	}

	return sptr;
}

/*
 * Used from output_syscall_prefix, and also from postmortem dumper
 */
static unsigned int render_syscall_prefix(struct syscallrecord *rec, char *bufferstart)
{
	struct syscallentry *entry;
	char *sptr = bufferstart;
	unsigned int i;
	unsigned int syscallnr;
	xmlChar *tmp;
	int rc;

	if(writer == NULL) {
	  writer = xmlNewTextWriterFilename("/data/local/tmp/asd", 0);
	  rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
	  if (rc < 0) {
	    printf
	      ("testXmlwriterFilename: Error at xmlTextWriterStartDocument\n");
	    return;
	  }
	}

	rc = xmlTextWriterStartElement(writer, BAD_CAST "SYSCALL");
	if (rc < 0) {
	  printf
	    ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
	  return;
	}
	
	syscallnr = rec->nr;
	entry = get_syscall_entry(syscallnr, rec->do32bit);

	sptr += sprintf(sptr, "[child%u:%u] [%lu] %s", this_child->num, this_child->pid,
			rec->op_nr,
			rec->do32bit == TRUE ? "[32BIT] " : "");

	sptr += sprintf(sptr, "%s%s(", entry->name, ANSI_RESET);	

	xmlTextWriterWriteAttribute(writer, "name", entry->name);

	if (tmp != NULL) xmlFree(tmp);

	for_each_arg(i) {
		sptr = render_arg(rec, sptr, i, entry);
	}

	sptr += sprintf(sptr, "%s) ", ANSI_RESET);


	
	/* Close the element named ENTRY. */
	rc = xmlTextWriterEndElement(writer);
	if (rc < 0) {
	  printf("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
	  return;
	}

	xmlTextWriterFlush(writer);
	fflush(NULL);


	return sptr - bufferstart;
}

static void flushbuffer(char *buffer, FILE *fd)
{
	fprintf(fd, "%s", buffer);
	fflush(fd);
}

static unsigned int render_syscall_postfix(struct syscallrecord *rec, char *bufferstart)
{
	char *sptr = bufferstart;

	if (IS_ERR(rec->retval)) {
		sptr += sprintf(sptr, "%s= %ld (%s)",
			ANSI_RED, (long) rec->retval, strerror(rec->errno_post));
	} else {
		sptr += sprintf(sptr, "%s= ", ANSI_GREEN);
		if ((unsigned long) rec->retval > 10000)
			sptr += sprintf(sptr, "0x%lx", rec->retval);
		else
			sptr += sprintf(sptr, "%ld", (long) rec->retval);
	}
	sptr += sprintf(sptr, "%s\n", ANSI_RESET);

	return sptr - bufferstart;
}

static void output_rendered_buffer(char *buffer)
{
	/* Output to stdout only if -q param is not specified */
	if (quiet_level == MAX_LOGLEVEL)
		flushbuffer(buffer, stdout);

	/* Exit if should not continue at all. */
	if (logging == TRUE) {
		FILE *log_handle;

		log_handle = find_logfile_handle();
		if (log_handle != NULL) {
			strip_ansi(buffer);
			flushbuffer(buffer, log_handle);
		}
	}
}

/* These next two functions are always called from child_random_syscalls() by a fuzzing child.
 * They render the buffer, and output it (to both stdout and logs).
 * Other contexts (like post-mortem) directly use the buffers.
 */
void output_syscall_prefix(struct syscallrecord *rec)
{
	static char *buffer = NULL;
	unsigned int len;

	if (buffer == NULL)
		buffer = zmalloc(PREBUFFER_LEN);

	len = render_syscall_prefix(rec, buffer);

	/* copy child-local buffer to shm, and zero out trailing bytes */
	memcpy(rec->prebuffer, buffer, len);
	memset(rec->prebuffer + len, 0, PREBUFFER_LEN - len);

	output_rendered_buffer(rec->prebuffer);
	sleep(5);
}

void output_syscall_postfix(struct syscallrecord *rec)
{
	static char *buffer = NULL;
	unsigned int len;

	if (buffer == NULL)
		buffer = zmalloc(POSTBUFFER_LEN);

	len = render_syscall_postfix(rec, buffer);

	/* copy child-local buffer to shm, and zero out trailing bytes */
	memcpy(rec->postbuffer, buffer, len);
	memset(rec->postbuffer + len, 0, POSTBUFFER_LEN - len);

	output_rendered_buffer(rec->postbuffer);
}
