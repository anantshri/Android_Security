#!/usr/bin/env python
import sys


blacklist = []

header = open('hooker.h', 'w')

hook_prototype = "void* no_proto(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;"
functions_ra = open(sys.argv[1], 'r').readlines()


header.write("struct hook_t *hook_hash1;\n\n")
header.write("unsigned int base_address;\n")
header.write( hook_prototype + "\n")


j=0
k=0
for i in functions_ra:

    if(  int(i.strip(),16)-1 in blacklist): continue 
    out = "#define HOOK_coverage_%d hook(hook_hash1, pid, \"libmedia\", \"\", no_proto, 0, %s );\n" % (j, i.strip())
    header.write(out)
    j+=1

    k+=1


header.close()

