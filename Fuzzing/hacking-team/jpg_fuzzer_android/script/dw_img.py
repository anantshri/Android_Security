import sys
import os
import string
from urllib import urlretrieve

f = open("./log", 'r')
c = 0
for i in f.readlines():
    print "Downloading %s \n" %i
    urlretrieve(i, str(c) + ".jpg")
    c = c+1
