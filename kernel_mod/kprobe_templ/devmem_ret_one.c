#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

static struct kretprobe my_kretprobe;
static const char *sym_name = "devmem_is_allowed";

static int ret_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	regs->ax = 1;
	return 0;
}

static __init int init_devmem_ret_one(void)
{
	int ret;
	printk("%s: sym_name: %s\n", __func__, sym_name);
	my_kretprobe.handler = ret_handler;
	my_kretprobe.kp.symbol_name = "devmem_is_allowed";
	if ((ret = register_kretprobe(&my_kretprobe)) < 0) {
		printk("register_kretprobe failed, returned %d\n", ret);
		return -1;
	}
	printk("Planted return probe at %p\n", my_kretprobe.kp.addr);
	return 0;
}
module_init(init_devmem_ret_one);

static __exit void cleanup_devmem_ret_one(void)
{
	printk("%s\n", __func__);
	unregister_kretprobe(&my_kretprobe);
}
module_exit(cleanup_devmem_ret_one);

MODULE_LICENSE("GPL");
