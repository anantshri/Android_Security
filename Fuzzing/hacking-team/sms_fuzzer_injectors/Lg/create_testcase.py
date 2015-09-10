import string
import os
import sys

def read_file(path, start, end):
    fw = open("test_case", "w")
    fr = open(path, "r");
    lines = fr.readlines()
    for i in range(start, end):
        fw.write(lines[i])
    
    fw.close()
    fr.close()
    

read_file(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]))
