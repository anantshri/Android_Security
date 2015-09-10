import os
import string
import sys


average_libxiv_len = 65

# class for a log file
class Hook:
	def __init__(self, test_case):
		self.file_id = test_case
		self.libxiv = []
		self.libskia = []
		self.libjpeg = []

	def add_hook(self, addr, lib):
		if lib.strip() == "libXIVCoder":
			self.libxiv.append(addr.strip())
		elif lib.strip() == "libskia":
			self.libskia.append(addr.strip())
		elif lib.strip() == "libjpeg":
			self.libjpeg.append(addr.strip())
			
# minset creation
def create_minset(hook_list):
	coverage = []
	minset = []
	global average_libxiv_len
	for h in hook_list:
		for i in h.libxiv:
			if (i not in coverage) or (len(h.libxiv) > average_libxiv_len):
				coverage = list(set(coverage+h.libxiv))
				minset.append(h)
				break
	return minset




hook = []
minset = []
hook_list = []


for (path, dirs, files) in os.walk("./minset/logs1"):
        pass

for i in files:
	f = open(path+"/"+i, 'r')
	h = Hook(i)
	hook_list.append(h)
	for l in f.readlines():
		hook = l.strip().split()
		if len(hook) == 4:
			h.add_hook(hook[0].strip(),hook[3].strip())
	#print "libxiv: %d libskia: %d libjpeg: %d file: %s" %(len(h.libxiv), len(h.libskia), len(h.libjpeg) , h.file_id) 


minset = create_minset(hook_list)
for i in minset:
	print i.file_id
