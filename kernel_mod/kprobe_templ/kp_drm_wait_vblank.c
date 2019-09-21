#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

struct probe_info {
	const char  *name;
	unsigned int offset;
};

static const struct probe_info symbols[] = {
	{"drm_wait_vblank", 0},
	{"drm_wait_vblank", 0x43},
	{"drm_wait_vblank", 0x50},
	{"drm_wait_vblank", 0x5d},
	{"drm_wait_vblank", 0xa5},
	{"drm_wait_vblank", 0xd5},
	{"drm_wait_vblank", 0x114},
	{"drm_control", 0},
};

#define NUM_SYMS sizeof(symbols) / sizeof (struct probe_info)

static struct kprobe kp_array[NUM_SYMS];
static int pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	printk(KERN_INFO "pre_handler[%d] %s [%lx]\n",
	       current->pid, p->symbol_name, regs->ip);
	//dump_stack();
	drm_irq_install();
	return 0;
}

static __init int init_mykp(void)
{
	int idx;
	int ret;
	for  (idx = 0; idx < NUM_SYMS; idx++) {
		struct kprobe *kp = &kp_array[idx];
		printk("%s: sym_name: %s (%x)\n", __func__,
		       symbols[idx].name, symbols[idx].offset);
		kp->pre_handler = pre_handler;
		kp->symbol_name = symbols[idx].name;
		kp->offset      = symbols[idx].offset;
		if ((ret = register_kprobe(kp)) < 0) {
			printk("register_kprobe failed, returned %d\n", ret);
			goto error;
		}
		printk("  => Addr: %p\n", kp->addr);
	}
	return 0;

error:
	for (idx--; idx >= 0; idx--) {
		struct kprobe *kp = &kp_array[idx];
		unregister_kprobe(kp);
	}
	return 0;
}
module_init(init_mykp);

static __exit void cleanup_mykp(void)
{
	int idx;
	for  (idx = NUM_SYMS - 1; idx >= 0; idx--) {
		struct kprobe *kp = &kp_array[idx];
		printk("[%s] Unregister: %s\n", __func__, kp->symbol_name);
		unregister_kprobe(kp);
	}
}
module_exit(cleanup_mykp);

MODULE_LICENSE("GPL");
