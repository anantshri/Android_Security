#include "syscall.h"
#include <libxml/xmlwriter.h>

extern int nullfd;

void struct_recursive_analyze(void *, enum struct_argtype, xmlTextWriterPtr);
