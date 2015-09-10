#!/usr/bin/python

# super ugly hack!

import os
import sys

fn = sys.argv[1]
fp = open(sys.argv[1]+".c", "w+")

fp.write("#include <stdio.h>\n\
#include <sys/types.h>\n\
#include <sys/stat.h>\n\
#include <fcntl.h>\n\
#include <dlfcn.h>\n\
#include \"libt.h\"\n\
#include \"util.h\"\n\
#include \""+fn+".h\"\n\n")

#fp.write("struct hook_t hook_" + fn + ";\n\n"\
#"struct special_" + fn + "_t {\n"\
#"\tpphLibNfc_RspCb_t *orig_cb;\n"\
#"\tpphLibNfc_RspCb_t *my_cb;\n"\
#"} special_" + fn + ";\n\n")

fp.write("NFCSTATUS\n"\
"(*orig_"+fn+")(...); //FIXME\n\n")

fp.write("/*\nvoid my_cb_"+fn+"(...) // FIXME\n"\
"{\n"\
"\tstruct special_"+fn+"_t *d = (struct special_"+fn+"_t*) hook_"+fn+".data;\n"\
"\tpphLibNfc_RspCb_t cb = d->orig_cb;\n"\
"\tlog(\"call %s\\n\", __func__)\n\n"\
"\tlog(\"call %s end\\n\", __func__)\n"\
"\tcb(pContext, Status);\n"\
"}\n*/\n\n")

fp.write("NFCSTATUS my_"+fn+"() // FIXME\n"\
"{\n"\
"\torig_"+fn+" = (void*) hook_"+fn+".orig;\n"\
"\tlog(\"%s enter\\n\", __func__)\n"\
"\tlog(\"orig_"+fn+ " = %x\\n\", orig_"+fn+")\n\n"\
"\tstruct special_" + fn+"_t *d = (struct special_"+fn+"_t*)hook_"+fn+".data;\n"\
"\td->orig_cb = X; // FIXME\n"\
"\td->my_cb = my_cb_"+fn+"; // FIXME\n"\
"\t//d->pContext = pContext;\n\n"\
"\thook_precall(&hook_"+fn+");\n"\
"\tNFCSTATUS res = orig_"+fn+"(...); // FIXME\n"\
"\thook_postcall(&hook_"+fn+");\n\n"\
"\tlog(\"%s result = %x\\n\", __func__, res)\n"\
"\treturn res;\n}\n")

fp.close()

fp = open(fn+".h", "w+")

fp.write("NFCSTATUS my_"+fn+"(); // FIXME\n\n")

fp.write("struct hook_t hook_" + fn + ";\n\n"\
"struct special_" + fn + "_t {\n"\
"\tpphLibNfc_RspCb_t orig_cb;\n"\
"\tpphLibNfc_RspCb_t my_cb;\n"\
"} special_" + fn + ";\n\n")

fp.write("#define HOOK_" + fn + " \\\n"\
"hook(&hook_"+fn+", pid, \"libnfc\", \""+fn+"\", my_"+fn+");\\\n"\
"hook_"+fn+".data = &special_"+fn+";\\\n"\
"memset((char*)&special_"+fn+", 0, sizeof(special_"+fn+"));")

fp.close()
