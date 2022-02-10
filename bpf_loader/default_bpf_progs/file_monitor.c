#include <linux/bpf.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/fs.h>

struct data_t {
    char name[DNAME_INLINE_LEN];
    u32 bytes;
    char op;
};

BPF_PERF_OUTPUT(data);

int write_monitor(struct pt_regs *ctx, struct file *file, size_t count){
    struct data_t bpf_data = {};
    bpf_data.op = 'w';
    bpf_data.bytes = count;
    if(!(PT_REGS_RC(ctx)))
        return 0;
    
    struct dentry *de = file->f_path.dentry;
	int mode = file->f_inode->i_mode;
	struct qstr d_name = de->d_name;
	if (d_name.len == 0 || !S_ISREG(mode)){
		return 0;
	}
    bpf_probe_read_kernel(&bpf_data.filename, sizeof(bpf_data.filename), d_name.name);
    data.perf_sumbit();

    return 1;
}

int read_monitor(struct pt_regs *ctx, struct file *file, size_t count){
    struct data_t bpf_data = {};
    bpf_data.op = 'r';
    bpf_data.bytes = count;
    if(!(PT_REGS_RC(ctx)))
        return 0;
    
    struct dentry *de = file->f_path.dentry;
	int mode = file->f_inode->i_mode;
	struct qstr d_name = de->d_name;
	if (d_name.len == 0 || !S_ISREG(mode)){
		return 0;
	}
    bpf_probe_read_kernel(&bpf_data.filename, sizeof(bpf_data.filename), d_name.name);
    data.perf_sumbit();

    return 1;
}