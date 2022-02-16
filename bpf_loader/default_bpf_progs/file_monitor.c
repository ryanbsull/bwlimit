#include <uapi/linux/bpf.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/fs_struct.h>
#include <linux/time.h>

#define BWLIM_GLOBAL 	(300)
#define REFRESH_TIME_NS	100000000
#define NS_PER_SEC	1000000000


BPF_PERF_OUTPUT(data);
BPF_ARRAY(limit, u64, 1);
BPF_ARRAY(time, u64, 1);
BPF_ARRAY(tp, u64, 1);

typedef struct data_t{
	u64 size;
	char filename[DNAME_INLINE_LEN];
} f_data;

static int delay(float ms){
	float i = 0, x = -5;

	for(; i < ms*1000000; i++)
		x = x + i - x*i;
	return 1;	
}

static int is_filter_fname(char filename[])
{
	return (filename[0] == 't' && filename[1] == 'e' && filename[2] == 's' && filename[3] == 't'
			&& filename[4] == '.' && filename[5] == 'b' && filename[6] == 'u' 
			&& filename[7] == 'f');
}

int wr_lim(struct pt_regs *ctx, struct file *file, size_t uk, size_t bytes){
	int key = 0;
	u64 *prev_ts, *lim, *dt, ts;
	f_data f = {};
	char* tgt_fname = "test.buf";	

	struct dentry *de = file->f_path.dentry;
	int mode = file->f_inode->i_mode;
	struct qstr d_name = de->d_name;
	if (d_name.len == 0 || !S_ISREG(mode)){
		return 0;
	}
	bpf_probe_read_kernel(&f.filename, sizeof(f.filename), d_name.name);

	lim = limit.lookup(&key);
	if(!lim)
		return -1;
	if(is_filter_fname(f.filename))
		*lim += bytes;
	
	f.size = *lim;
	if(*lim > BWLIM_GLOBAL){
		*lim = 0;
		delay(10000000);
		data.perf_submit(ctx, &f, sizeof(f));
	}

	prev_ts = time.lookup(&key);
	if(!prev_ts)
		return -1;
	
	dt = tp.lookup(&key);
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
