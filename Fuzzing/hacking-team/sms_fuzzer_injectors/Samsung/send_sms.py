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
    
    toSend = binascii.unhexlify(sms.strip()).strip()

    s = socket(AF_INET, SOCK_STREAM )

    s.connect((serverHost, serverPort)) 
    s.send(struct.pack('i', len(toSend) + 1))
    s.send(toSend)               

    time.sleep(0.1)
    
    s.close()


f = open(sys.argv[1], "r");


for i in f.readlines():
    send_sms(i, int(sys.argv[2]))
    



