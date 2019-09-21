#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

static struct kprobe kp;
static const char *sym_name = "igd_alter_ovl2";

static int pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	printk(KERN_INFO "pre_handler [%s]\n", sym_name);
	//dump_stack();
	return 0;
}

static __init int init_mykp(void)
{
	int ret;
	printk("%s: sym_name: %s\n", __func__, sym_name);
	kp.pre_handler = pre_handler;
	kp.symbol_name = sym_name;
	if ((ret = register_kprobe(&kp)) < 0) {
		printk("register_kprobe failed, returned %d\n", ret);
		return -1;
	}
	printk("Planted return probe at %p\n", kp.addr);
	return 0;
}
module_init(init_mykp);

static __exit void cleanup_mykp(void)
{
	printk("%s\n", __func__);
	unregister_kprobe(&kp);
}
module_exit(cleanup_mykp);

MODULE_LICENSE("GPL");
