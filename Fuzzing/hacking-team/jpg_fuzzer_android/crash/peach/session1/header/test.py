import os
import sys
import string
import struct
import shutil


for i in range(0x0,0xff):
    shutil.copyfile("sample.jpg","fuzzed/fuzz"+str(i)+".jpg")
    with file("fuzzed/fuzz"+str(i)+".jpg", 'rb+') as fh:
        fh.seek(0xaa)
        fh.write(struct.pack('>B', i))
