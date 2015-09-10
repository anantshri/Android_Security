import xml.etree.ElementTree as ET
import base64
import sys

dev = "/dev/kgsl-3d0"
f = open("test-case.c", "w")

head = "#include <sys/ioctl.h>\n#include <stdio.h>\n#include <string.h>\n#include <dlfcn.h>\n#include <sys/time.h>\n#include <fcntl.h>\n\ntypedef int bool;\n#define true 1\n#define false 0\n\n#include \"ioctl_types.h\"\n\nint main(int argc, char *argv[]) {\n\n int fd = open(\""

head = head + dev + "\", O_RDWR);\n if(fd < 0)\nprintf(\"Error\\n\");\n\n"

f.write(head)

tree = ET.parse(sys.argv[1])
root = tree.getroot()

ioctl = ""
address = ""
ioctl_call = []
for child_id,child in enumerate(reversed(root.findall('SYSCALL'))):
#for child_id,child in enumerate(root.findall('SYSCALL')):
    for i in child.findall('ARG'):
        if i.get("name") == "cmd":
            ioctl = list(i)[0].text

        if i.get("name") == "arg":
            for c in i:
                if c.tag == "ARG_ADDRESS":
                    address = c.text

                if "STRUCT_" in c.tag:
                    if c.text == "unmapped":
                        f.write("void *arg_" + str(child_id)  + "= (void *) " + address + "; //unmapped \n")
                        continue
                        
                    
                    for cc_id,cc in enumerate(c):
                        if cc.tag == "CONTENT_DUMP":
                            f.write("struct " + c.tag[7:] + " *arg_" + str(child_id) + ";\n")
                            f.write("char dump" + str(child_id) + "_" + str(cc_id) + "[] = \"")
                            for i in bytearray(base64.b64decode(cc.text)):
                                f.write("\\x%02x" %(i))
                            f.write("\";\n")
                            f.write("arg_" + str(child_id)  + " = (struct " + c.tag[7:] + " *) dump" + str(child_id) + "_" + str(cc_id) + ";\n\n")
                        
                        if "POINTER_" in cc.tag:
                            if cc.text == "unmapped":
                                continue
        
                            f.write("char dump" + str(child_id) + "_" + str(cc_id) + "[] = \"")
                            for i in bytearray(base64.b64decode(cc.text)):
                                f.write("\\x%02x" %(i))
                            f.write("\";\n")
                            f.write("arg_" + str(child_id) + "->" + cc.tag[8:] + " = dump" + str(child_id) + "_" + str(cc_id) + ";\n\n")


    ioctl_call.append("ioctl(fd, " + ioctl + ", (void *)arg_" + str(child_id) + ");\n")
    
    if child_id == int(sys.argv[2]):
        break


for i in ioctl_call:
    f.write(i)

f.write("\n}\n")    

f.close()
