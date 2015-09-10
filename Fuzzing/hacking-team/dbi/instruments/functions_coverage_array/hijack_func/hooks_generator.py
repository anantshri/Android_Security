#!/usr/bin/env python
import sys

blacklist = [0x716d4, 0x71688, 0x71264, 0x6de70, 0x6de4c, 0x718b4, 0x717ac, 0x6de28, 0x6ddfc, 0x6ddd8, 0x6dd98, 0x6dd34, 0x6dcd0, 0x6dc94, 0x6dc5c, 0x6dbd8, 0x6e21c, 0x6db8c, 0x6db5c, 0x7162c, 0x6dabc, 0x6da54, 0x6da08, 0x6d9c8, 0x6d988, 0x6d938, 0x6d8f4, 0x6d8b4, 0x6d884, 0x6d828, 0x6d7fc, 0x6d7c0, 0x6d788, 0x6d74c, 0x6d71c, 0x6d6f8, 0x6d6d0, 0x6bb60, 0x6bb64, 0x6bb68, 0x6df10, 0x6e2a8, 0x6e844, 0x714f4, 0x714b0, 0x71400, 0x71354, 0x6d67c, 0x71270, 0x6d600, 0x7116c, 0x6d5c0, 0x6d55c, 0x6d4ec, 0x6d488, 0x6d444, 0x6d414, 0x6d38c, 0x6d354, 0x710e0, 0x6d2f0, 0x6d2ac, 0x70f6c, 0x6d240, 0x6d1e8, 0x6d1ac, 0x6d184, 0x6d148, 0x6d108, 0x6d0c8, 0x6d074, 0x6d008, 0x6cfc0, 0x6cf5c, 0x6ce74, 0x6ce50, 0x6ce28, 0x6ce04, 0x6cddc, 0x6cd94, 0x6cd54, 0x6cce0, 0x6cc98, 0x6cc28, 0x6cbe8, 0x6cbac, 0x6cb74, 0x6cb34, 0x6caf4, 0x70e88, 0x6cab8, 0x6ca78, 0x6ca3c, 0x6c9fc, 0x6c9d4, 0x6c9b0, 0x6c974, 0x6c94c, 0x6c914, 0x6c8e8, 0x6c894, 0x6c86c, 0x6c848, 0x6c798, 0x6c770, 0x6c748, 0x6c724, 0x6c6dc, 0x6c6ac, 0x6c67c, 0x6c634, 0x6c5f0, 0x6c5a8, 0x6c564, 0x6c540, 0x6c518, 0x6c4f0, 0x6c434, 0x6c3b0, 0x70d70, 0x70bf4, 0x70978, 0x70904, 0x70780, 0x706a8, 0x70698, 0x70690, 0x70684, 0x7054c, 0x70368, 0x7027c, 0x70254, 0x701d8, 0x700f4, 0x70084, 0x70008, 0x70000, 0x6ffd8, 0x6ffb4, 0x6ff7c, 0x6c264, 0x6fed4, 0x6ff74, 0x6fedc, 0x6fecc, 0x6fdac, 0x6fd70, 0x6fbe8, 0x6f7d4, 0x6f6f8, 0x6f51c, 0x6f498, 0x6f4ac, 0x6f414, 0x6f428, 0x6f274, 0x6f208, 0x6f14c, 0x6f0e0, 0x6f040, 0x6eff8, 0x6eff0, 0x6c22c, 0x6efe8, 0x6c1cc, 0x6ee80, 0x6c120, 0x6bff4, 0x6bef8, 0x6ee50, 0x6ee20, 0x6edcc, 0x6beac, 0x6edc4, 0x6edbc, 0x6edb4, 0x6eda4, 0x6bb6c, 0x6ed9c, 0x6be20, 0x6e808, 0x6bde8, 0x6bdb4, 0x6bd70, 0x6debc, 0x6bcac, 0x6bca4, 0x6bc9c, 0x6bc94, 0x6bc04 ] 

header = open('hooker.h', 'w')

hook_prototype = "void* no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p) ;"
functions_ra = open(sys.argv[1], 'r').readlines()


header.write("struct hook_t *hook_hash1;\n\n")
header.write("unsigned int base_address;\n")
header.write( hook_prototype + "\n")


j=0
k=0
for i in functions_ra:

    if( int(i.strip(),16) < 0xc9c50 or  int(i.strip(),16) > 0x25597e or int(i.strip(),16)-1 in blacklist): continue # int(i.strip(),16) < 0x11bb59 or int(i.strip(),16) > 0x45597e or
    out = "#define HOOK_coverage_%d hook(hook_hash1, pid, \"libpolarisoffice\", \"\", no_proto, 0, %s );\n" % (j, i.strip())
    header.write(out)
    j+=1

    k+=1


out = 'struct dispatcher_t *dispatcher_array[%d];' % (j+1)

header.write(out)

header.close()



exit(1)

c = open('hooker_thumb.c', 'w')

c.write('#include <stdio.h>\n' \
            '#include <sys/types.h>\n' \
            '#include <sys/stat.h>\n' \
            '#include <fcntl.h>\n' \
            '#include <dlfcn.h>\n' \
            '#include "libt.h"\n' \
            '#include "util.h"\n' \
            '#include "hooker.h"\n\n' )

c.write('void* (*coverage_ptr)(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p);\n\n')

j=0
for i in functions_ra:

    
    out = 'void*  coverage_%d(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p) {\n' % j 
    c.write( out )

    c.write('\tvoid *result;\n')

    out = '\tcoverage_ptr = (void *) hook_array[%d].orig;\n\n' % j
    c.write( out  )

    c.write('\tlog("[*]\\tHit func 0x%x\\n", coverage_ptr)\n')


    out = '\thook_precall(&hook_array[%d]);\n' % j
    c.write(out)

    c.write('\tresult = coverage_ptr( a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p);\n')
 
    out = '\thook_postcall(&hook_array[%d]);\n' % j
    c.write(out)

    c.write('\treturn result;\n')
    c.write('}\n\n')

    j+=1

c.close()
