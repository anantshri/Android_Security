import os
import sys
import string

s = "HOOK_coverage_"
f = open("hook_list","w")

for i in range(0,int(sys.argv[1],10)):
    f.write(s + str(i) + "\n")
