#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>

static __init int init_mod_ex(void)
{
	printk("%s: INIT\n", __func__);
	return 0;
}
module_init(init_mod_ex);

static __exit void cleanup_mod_ex(void)
{
	printk("%s: EXIT\n", __func__);
}
module_exit(cleanup_mod_ex);

MODULE_LICENSE("GPL");
