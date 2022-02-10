#from bcc import BPF
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

file_monitors = BPF(src_file='default_bpf_progs/file_monitor.c')
file_monitors.attach_kprobe(event='vfs_write', fn_name='write_monitor')
file_monitors.attach_kprobe(event='vfs_read', fn_name='read_monitor')

buf = open("buf_test/buffer","r")
arglines = follow(buf)
for line in arglines:
    args = line.split(',')
