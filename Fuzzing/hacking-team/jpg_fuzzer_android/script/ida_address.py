import string
from idautils import *
from idc import *

MAX_THUMB_SIZE = 22
MAX_ARM_SIZE = 16
THUMB_INSTR_SIZE = 2

bl_quram = [0x3bea0, 0x6090c, 0x60a78, 0x60b04, 0x9b724]


def isThumb(func_start, func_end):
    res = False
    for i in Heads(func_start, func_end):
	    if ItemSize(i) == THUMB_INSTR_SIZE:
		    res = True
		    break
    return res


ea = ScreenEA();
dump = open("E:/test", 'a')
dump.write("@" + GetInputFile()[:-3].strip() + "\n")
current_f = GetInputFile()[:-3].strip()

bl = []
'''a = CodeRefsTo(0x3c684,0)
for i in a:
     bl.append(GetFunctionName(i))'''


# black-list creation
for funcea in Functions():
    if "pthread_" in GetFunctionName(funcea) or "Thread" in GetFunctionName(funcea):
        a = CodeRefsTo(funcea,0)
        for i in a:
            bl.append(GetFunctionName(i))


# dumping thumb functions... 
print "Starting dump for thumb functions.."
funcea_end = 0
for funcea in Functions():
    if current_f == "libXIVCoder" and funcea in bl_quram:
        print "%x blacklisted \n" %funcea
        continue
    if(GetFunctionName(funcea) in bl):
        print "%s discarded" %GetFunctionName(funcea)
        continue
    funcea_end = FindFuncEnd(funcea)
    if( isThumb(funcea, funcea_end) and funcea_end - funcea > MAX_THUMB_SIZE ):
        out = "0x%x\n" % (funcea + 1)
        dump.write( out )

print "End dump for thumb.."


# dumping arm functions... 
print "Starting dump for ARM functions.." 
funcea_end = 0
for funcea in Functions():
    if current_f == "libXIVCoder" and funcea in bl_quram:
        print "%x blacklisted \n" %funcea
        continue
    if(GetFunctionName(funcea) in bl):
        print "%s discarded" %GetFunctionName(funcea)
        continue
    funcea_end = FindFuncEnd(funcea)
    if( not isThumb(funcea, funcea_end) and funcea_end - funcea > MAX_ARM_SIZE ):
        out = "0x%x\n" % (funcea)
        dump.write( out ) 

print "End dump for ARM.."

dump.close()
