import sys

f = open(sys.argv[1], "r")
w = sys.argv[1].strip().split(".")
f2 = open(w[0] + "-fixed.xml", "w")


for c,i in enumerate(f.readlines()):
    if c == 1:
        f2.write("<FUZZED_SYSCALLS>" + i.replace("\r\n", ""))
    else:
        f2.write(i.replace("\r\n", ""))

f2.write("</FUZZED_SYSCALLS>");
