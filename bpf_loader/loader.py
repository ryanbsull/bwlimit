from bcc import BPF
import ctypes as ct
from struct import pack
import time

def follow(thefile):
    thefile.seek(0,2)
    while True:
        line = thefile.readline()
        if not line:
            time.sleep(0.1)
            continue
        yield line

byte_pos = 0
logfile = open("buf_test/buffer","r")
loglines = follow(logfile)
for line in loglines:
    print(line)