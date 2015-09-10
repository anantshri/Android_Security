#!/usr/bin/env python
import sys
header = open('hooker.h', 'w')

hook_thumb_prototype = "void* no_proto_thumb(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;"

hook_arm_prototype = "void* no_proto_arm(void* a, void* b, void* c, void* d, void* e, void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u, void* w) ;"

hook_readjpeg_prototype = "void* read_jpeg(void* a, void* b);"
hook_access_prototype = "void* access_arm(void* a, void* b);"


functions_ra = open(sys.argv[1], 'r').readlines()

header.write("#include <pthread.h>\n\n");
header.write("struct hook_t *hook_hash1;\n")
header.write("struct hook_t *hooked_list_hash;\n\n")
header.write("unsigned int base_address;\n")
header.write("pthread_mutex_t M;\n");
header.write("int tid_locker;\n\n");
header.write( hook_arm_prototype + "\n\n")
header.write( hook_thumb_prototype + "\n\n")
header.write( hook_readjpeg_prototype + "\n\n")
header.write( hook_access_prototype + "\n\n")

j=0
k=0
current_lib = ""
for i in functions_ra:
    if(i[0] == "@"):
        current_lib = i.strip()[1:]
        continue

    if(int(i.strip(),16) % 4 == 0):
           out = "#define HOOK_coverage_%d hook(hook_hash1, pid, \"%s\", \"\", no_proto_arm, 0, %s );\n" % (j, current_lib, i.strip())
    else:
           out = "#define HOOK_coverage_%d hook(hook_hash1, pid, \"%s\", \"\", no_proto_thumb, 0, %s );\n" % (j, current_lib, i.strip())
    header.write(out)
    j+=1
    k+=1

out = "#define HOOK_coverage_%d hook(hook_hash1, pid, \"libexif\", \"\", read_jpeg, 0, 0x5229 );\n" % j
out2 = "#define HOOK_coverage_%d hook(hook_hash1, pid, \"libc\", \"\", access_arm, 0, 0xd0e4 );\n" % (j+1)
header.write(out)
header.write(out2)
header.close()
