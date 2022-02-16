from bcc import BPF
import ctypes as ct
import time

test = BPF(src_file="default_bpf_progs/file_monitor.c")
test.attach_kprobe(event="vfs_write", fn_name="wr_lim")
limit = 300

def log_write_event(cpu,data,size):
    event = test["data"].event(data)
    e = "SIZE: %s \t FILE: %s \n" % (event.size, event.filename.decode('utf-8', 'replace'))
    print(e)
    if event.size > limit:
        time.sleep(1.0)

test["data"].open_perf_buffer(log_write_event)


while True:
    test.perf_buffer_poll()
