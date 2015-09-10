#!/usr/bin/env python
import sys


header = open('hooker.h', 'w')

hook_prototype = "void*  coverage(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r) ;"
functions_ra = open(sys.argv[1], 'r').readlines()


header.write("struct hook_t hook_array[%d];\n\n" % len(functions_ra))
header.write( hook_prototype + "\n")


j=0
for i in functions_ra:
    out = "#define HOOK_coverage_%d hook(&hook_array[%d], pid, \"libpolarisoffice\", \"\", coverage_%d, 0, %s );\n" % (j, j, j, i.strip())
    header.write(out)
    j+=1



header.close()

#exit(0)

c = open('hooker_thumb.c', 'w')

c.write('#include <stdio.h>\n' \
            '#include <sys/types.h>\n' \
            '#include <sys/stat.h>\n' \
            '#include <fcntl.h>\n' \
            '#include <dlfcn.h>\n' \
            '#include "libt.h"\n' \
            '#include "util.h"\n' \
            '#include "hooker.h"\n\n' )

c.write('void* (*coverage_ptr)(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r);\n\n')

j=0
for i in functions_ra:

    
    out = 'void*  coverage_%d(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r) {\n' % j 
    c.write( out )

    c.write('\tvoid *result;\n')

    out = '\tcoverage_ptr = (void *) hook_array[%d].orig;\n\n' % j
    c.write( out  )

    c.write('\tlog("[*]\\tHit func 0x%x\\n", coverage_ptr)\n')


    out = '\thook_precall(&hook_array[%d]);\n' % j
    c.write(out)

    c.write('\tresult = coverage_ptr( a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r);\n')
 
    out = '\thook_postcall(&hook_array[%d]);\n' % j
    c.write(out)

    c.write('\treturn result;\n')
    c.write('}\n\n')

    j+=1

c.close()
