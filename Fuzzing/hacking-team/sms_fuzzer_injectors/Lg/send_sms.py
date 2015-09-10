#!/usr/bin/python

import sys
import binascii
from socket import *
import struct
import time

count = 0

def send_sms(sms, port):
    global count

    count = count + 1
    
    print "Sending sms number %d" %count

    serverHost = "127.0.0.1"
    serverPort = port
    
    toSend = sms.strip()
    length = (len(toSend)/2) - 8
    header = "+CMT: ," + str(length) + "\n"

    s = socket(AF_INET, SOCK_STREAM )

    s.connect((serverHost, serverPort)) 
    
    s.send(header + toSend + "\n")               

    time.sleep(0.1)
    
    s.close()


f = open(sys.argv[1], "r");

for i in f.readlines():
    send_sms(i, int(sys.argv[2]))
    



