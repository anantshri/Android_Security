#!/usr/bin/python

import sys
import os
from lexer import *
from string import *

# Build the lexer
lexer = lex.lex()

primitive = [
  ["uint32_t"],
  ["int32_t"],
  ["int"],
  ["char"],
  ["unsigned int"],
  ["unsigned long"],
  ["size_t"],
  ["__u32"],
  ["__s32"],
  ["__u16"],
  ["__u8"]
]


def isPrimitive(name):
  for i in primitive:
    if i[0] == name:
      return True

  return False


######################## FUZZ PROCEDURES CREATION ################
##################################################################


# Open files to be read/writen
#file0 = open("manual_types","r")
file1 = open("ioctl_types.h","r")
file2 = open("fuzz_ioctl_struct.c","w")
file3 = open("struct_type_enum","w")

tokens1 = []
tokens2 = []
type_list = []
struct_list = []
# Tokenize the source list of structs
for line in file1.readlines():
  lexer.input(line)
  while True:
    tok = lexer.token()
    if not tok: break      
    tokens1.append(tok)

tokens2 = list(tokens1)

last = None
k = 0
l = []
b = False
for i in tokens2:
  if i.type == "STRUCT" and tokens2[k+2].type == "OPEN":
    #type_list.append(tokens2[k+1].value)
    last = tokens2[k+1].value
    struct_list.append(tokens2[k+1].value)

  elif i.type == "OPEN":
    b = True
    k = k + 1
    continue

  elif i.type == "CLOSE":
    
    type_list.append((last,l))
    b = False
    l = []
    k = k + 1
    continue

  if b == True:
    l.append(i)

  k = k + 1



k = 0
for i in tokens2:
  if i.type == "STRUCT" and tokens2[k+1].type != "OPEN" and tokens2[k+2].type != "OPEN":
     if tokens2[k+1].value not in struct_list:
       print "WARNING: struct %s is not defined as a type" %(tokens2[k+1].value)
  k = k + 1



file2.write("/* Automatically generated */\n#include <stdlib.h>\n#include \"child.h\"\n#include \"ioctls.h\"\n#include \"ioctl_types.h\"\n#include \"maps.h\"\n#include \"random.h\"\n#include \"sanitise.h\"\n#include \"shm.h\"\n#include \"syscall.h\"\n#include \"trinity.h\"\n#include \"fuzz_ioctl_struct.h\"\n\n")

file2.write("static void * recursive_fuzz(enum ioctl_struct_type type, void * local_addr) {\n\nswitch(type) {\n")


for i in struct_list:
  file3.write("STRUCT_" + i + ",\n")

k = 0
for i in type_list:
  file2.write("struct " + i[0] + " *p" + str(k) + ";\n")
  k = k + 1

file2.write("\n\n")

k = 0
for i in type_list:
  
  file2.write("case STRUCT_" + i[0] + ":\n")
  file2.write("if(local_addr != NULL)\n" + "p" + str(k) + " = (struct " + i[0] + " *)local_addr;\nelse\n")
  file2.write("p" + str(k) + " = (struct " + i[0] + " *)get_writable_address(sizeof(struct " + i[0] + "));\n")
  file2.write("if(p" + str(k) + ") {\n")
  ################ PARSING #####################
  t = i[1]
  y=0
  for tt in t:

    if t[y].type == "ENUM":
      file2.write("//enum " + t[y+1].value + ";\n")

    if t[y].type == "STRUCT" and t[y+1].type == "NAME":
      if t[y+3].type == "ARRAY" and t[y+2].type == "NAME":
        for kk in range(0, int(t[y+3].value[1:-1])):
          file2.write("recursive_fuzz(STRUCT_" + t[y+1].value + ", &p" + str(k) + "->" + t[y+2].value + "[" + str(kk) + "]);\n")

      elif t[y+2].type == "NAME":
        file2.write("recursive_fuzz(STRUCT_" + t[y+1].value + ", &p" + str(k) + "->" + t[y+2].value + ");\n")

      elif t[y+2].type == "POINTER":
        file2.write("p" + str(k) + "->" + t[y+2].value[1:] + " = (struct " + t[y+1].value + " *) recursive_fuzz(STRUCT_" + t[y+1].value +", NULL);\n")
        


    if t[y].type == "NAME" and t[y+1].type == "NAME" and t[y+2].type == "END":
      if isPrimitive(t[y].value):
        file2.write("//%s is primitive\n" % (t[y+1].value))
        #print("%s is primitive\n" % (t[y+1]))

    elif t[y].type == "NAME" and t[y+1].type == "POINTER":
      if t[y].value == "void":
        #print("%s is a pointer to %s\n" % (t[y+1], t[y+0]))
        file2.write("p" + str(k) + "->" + t[y+1].value[1:] + " = (void *) get_non_null_address();\n")
      elif isPrimitive(t[y].value):
        file2.write("p" + str(k) + "->" + t[y+1].value[1:] + " = (" + t[y].value + " *) get_writable_address(sizeof(" + t[y].value + "));\n")
      #else:
      #file2.write("p" + str(k) + "->" + t[y+1].value[1:] + " = (" + t[y].value + " *) recursive_fuzz(STRUCT_" + t[y].value +");\n")

    elif t[y].type == "NAME" and t[y+1].type == "NAME" and t[y+2].type == "ARRAY":
      #print("%s is an array of %s of %s elements\n" % (t[y+1], t[y+0], t[y+2].value[1:-1]))
      file2.write("//%s is an array of %s of %s elements\n" % (t[y+1].value, t[y+0].value, t[y+2].value[1:-1]))

    y = y + 1



  ##############################################
  file2.write("return p" + str(k) + ";\n}\nreturn get_non_null_address();")
  file2.write("\n\n\n")
  k = k + 1

file2.write("default:\nreturn get_non_null_address();\n}\n}\n\n")

file2.write("void fuzz_ioctl_struct_type(struct syscallrecord *rec) {\n\n// Sanity check\nif(!rec->ioctl_struct_type) {\nif(rand_bool()) {\nrec->a3 = rand32();\nreturn;\n}\n\nrec->a3 = (unsigned long) get_non_null_address();\nreturn;\n}\n\nrec->a3 = (unsigned long) recursive_fuzz(rec->ioctl_struct_type, NULL);\n\nreturn;\n}")


file1.close()
file2.close()
file3.close()


####################### DUMP PROCEDURES CREATION ###################
####################################################################

head = "/***** Automatically generated by gen_recursive.c *********/\n\n#include <stdio.h>\n#include <time.h>\n#include <sys/types.h>\n#include <sys/stat.h>\n#include <fcntl.h>\n#include <sys/mman.h>\n#include <libxml/xmlwriter.h>\n#include <libxml/encoding.h>\n#include <sys/times.h>\n#include <sched.h>\n#include <linux/kexec.h>\n#include <sys/time.h>\n#include <sys/resource.h>\n#include <sys/vfs.h>\n#include <sys/syscall.h>\n#include <linux/aio_abi.h>\n#include <unistd.h>\n#include <utime.h>\n#include <dirent.h>\n#include <stdlib.h>\n#include <netinet/in.h>\n#include <poll.h>\n#include <sys/epoll.h>\n#include <sys/utsname.h>\n#ifdef LOLLIPOP\n#include <sys/msg.h>\n#include <sys/timex.h>\n#endif\n#include <sys/ipc.h>\n#include <linux/shm.h>\n#include <linux/futex.h>\n#include <linux/fs.h>\n#include <linux/sysctl.h>\n\n#include \"struct_analyze.h\"\n#include \"syscall.h\"\n#include \"ioctl_types.h\"\n\nint nullfd = 0;\n\nvoid struct_recursive_analyze(void *arg_ptr, bool is_ioctl_call, enum ioctl_struct_type ioctl_struct_type, enum struct_argtype struct_argtype, xmlTextWriterPtr writer) {\n\nif(arg_ptr == NULL)\nreturn;\nif(nullfd == 0)\nnullfd = open(\"/dev/random\", O_WRONLY);\n\nif(is_ioctl_call == TRUE) {\nswitch(ioctl_struct_type) {\n"


f = open("test.c", "w")
f.write(head)

k = 0
for i in struct_list:
  f.write("struct " + i + " *p" + str(k) + ";\n")
  k = k + 1


f.write("case STRUCT_undefined:\n\nreturn;\n\n")


k = 0
for i in type_list:

  # STRUCT DUMP
  f.write("case STRUCT_" + i[0] + ":\n")
  f.write("if(write(nullfd, (void *) arg_ptr, sizeof(struct " + i[0] + ")) < 0) {\n")
  f.write("xmlTextWriterWriteElement(writer, \"STRUCT_" + i[0] + "\", \"unmapped\");\n")
  f.write("return;\n }\n\n")
  
  f.write("p" + str(k) + " = (struct " + i[0] + " *) arg_ptr;\n")
  f.write("xmlTextWriterStartElement(writer, \"STRUCT_" + i[0] + "\");\n")
  f.write("xmlTextWriterStartElement(writer, \"CONTENT_DUMP\");\n")
  f.write("xmlTextWriterWriteBase64(writer, (char *) " + "p" + str(k) + " , 0, sizeof(struct " + i[0] + "));\n")
  f.write("xmlTextWriterEndElement(writer);\n\n")
  
  ################### CHECK FOR POINTERS ############################
  t = i[1]
  y=0
  for tt in t:

    # Pointer to STRUCT
    if t[y].type == "STRUCT" and t[y+1].type == "NAME" and t[y+2].type == "POINTER":
      f.write("// Pointer to struct " + t[y+1].value + "\n")
      f.write("struct_recursive_analyze((void *)(p" + str(k) + "->" + t[y+2].value[1:] + "), TRUE, STRUCT_" + t[y+1].value + ", 0, writer);\n")
      #f.write("p" + str(k) + "->" + t[y+2].value[1:] + " = (struct " +  t[y+1].value + " *) recursive_fuzz(STRUCT_" + t[y+1].value +", NULL);\n")

    if t[y].type == "NAME" and t[y+1].type == "POINTER":

      # Void pointer
      if t[y].value == "void":
        f.write("// void pointer\n")
        f.write("if(write(nullfd, (void *)" + "(p" + str(k) + "->" + t[y+1].value[1:] + "), 128) >= 0) {\n")                
        f.write("xmlTextWriterStartElement(writer, \"POINTER_" + t[y+1].value[1:] + "\");\n")
        f.write("xmlTextWriterWriteBase64(writer, (char *) " + "(p" + str(k) + "->" + t[y+1].value[1:] + "), 0, 128);\n")
        f.write("xmlTextWriterEndElement(writer);\n}\n")
        f.write("else\n")
        f.write("xmlTextWriterWriteElement(writer, \"POINTER_" + t[y+1].value[1:] + "\", \"unmapped\");\n")

      # Pointer to primitive
      elif isPrimitive(t[y].value):        
        f.write("// pointer to primitive\n")
        f.write("if(write(nullfd, (void *)" + "(p" + str(k) + "->" + t[y+1].value[1:] + "), 4) >= 0) {\n")                
        f.write("xmlTextWriterStartElement(writer, \"POINTER_" + t[y+1].value[1:] + "\");\n")
        f.write("xmlTextWriterWriteBase64(writer, (char *) " + "(p" + str(k) + "->" + t[y+1].value[1:] + "), 0, 4);\n")
        f.write("xmlTextWriterEndElement(writer);\n}\n")
        f.write("else\n")
        f.write("xmlTextWriterWriteElement(writer, \"POINTER_" + t[y+1].value[1:] + "\", \"unmapped\");\n")


        #f.write("p" + str(k) + "->" + t[y+1].value[1:] + " = (" + t[y].value + " *) get_writable_address(sizeof(" + t[y].value + "));\n")
      #else:
      #file2.write("p" + str(k) + "->" + t[y+1].value[1:] + " = (" + t[y].value + " *) recursive_fuzz(STRUCT_" + t[y].value +");\n")
    
    y = y + 1
    ###################################################################

  f.write("xmlTextWriterEndElement(writer);\n\n")
  f.write("return;\n\n")
  f.write("\n")
  k = k + 1

f.write("\n}\n}\n}\n")

f.close()
