import os
import sys
import string

f = open("addr2","r")

bl = ["00089908", "0008ba50", "00089b24", "00089450", "0008cca8", "00090790", "0007483c", "00089054"]
crashes = []

for i in f.readlines():
    if i.split("!")[1].strip() in bl:
        continue
    crashes.append((i.split("!")[0].strip(),i.split("!")[1].strip()))


for i in list(set(crashes)):
    print i
