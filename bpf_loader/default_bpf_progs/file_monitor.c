#include <linux/bpf.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/fs.h>
#include <unistd.h>

#define BWLIM_GLOBAL (5 * 1024 * 1024)
#define REFRESH_TIME_NS	100000000
#define NS_PER_SEC	1000000000

BPF_PERF_OUTPUT(data);
BPF_ARRAY(limit, u64, 1);
BPF_ARRAY(time, u64, 1);
BPF_ARRAY(tp, u64, 1);

int wr_lim(size_t bytes){
	u64 key = 0, *prev_ts, *lim, *dt, ts;
	
	lim = limit.lookup(&key);
    if(!lim)
        return -1;
	*lim += bytes;

	prev_ts = time.lookup(&key);
    if(!prev_ts)
        return -1;

	if(*lim > BWLIM_GLOBAL){
		*lim = 0;
		usleep(1000);
	}
    dt = tp.lookip(&key);
    if(!dt)
        return -1;

	ts = bpf_ktime_get_ns();
	*dt += ts - *prev_ts;
    *prev_ts = ts;
    
	if(*dt > REFRESH_TIME_NS){
		*dt = 0;
		*lim = 0;
	}
	return 0;
}
