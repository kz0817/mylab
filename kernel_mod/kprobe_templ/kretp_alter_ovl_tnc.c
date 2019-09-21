#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

static struct kretprobe my_kretprobe;
static const char *sym_name = "alter_ovl_tnc";

static int ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	printk(KERN_INFO "ret_handler: regs->ax: %lx\n", regs->ax);
	return 0;
}

static __init int init_myprobe(void)
{
	int ret;
	printk("%s: sym_name: %s\n", __func__, sym_name);
	my_kretprobe.handler = ret_handler;
	my_kretprobe.kp.symbol_name = sym_name;
	if ((ret = register_kretprobe(&my_kretprobe)) < 0) {
		printk("register_kretprobe failed, returned %d\n", ret);
		return -1;
	}
	printk("Planted return probe at %p\n", my_kretprobe.kp.addr);
	return 0;
}
module_init(init_myprobe);

static __exit void cleanup_myprobe(void)
{
	printk("%s\n", __func__);
	unregister_kretprobe(&my_kretprobe);
}
module_exit(cleanup_myprobe);

MODULE_LICENSE("GPL");
