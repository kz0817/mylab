#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/fb.h>

//struct jprobe jp0;
struct jprobe jp;

long
jp_drm_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_INFO "cmd: %x, arg: %lx\n", cmd, arg);
	jprobe_return();
	return 0;
}
/*
int
jp_fb_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
	unsigned long arg) 
{
	printk("cmd: 0x%x\n", cmd);
	jprobe_return();
	return 0;
}*/

/*
int jp_fb_set_var(struct kprobe *p, struct pt_regs *regs) {
  //printk("pre_handler: p->addr=0x%p, eflags=0x%lx\n",p->addr,
  //  regs->eflags);
  int i;
  dump_stack();
  unsigned long* sp = (unsigned long*)regs->esp;
  unsigned long ret = *sp;
  unsigned long* a_info = (unsigned long*) *(sp+1);
  unsigned long* a_var  = (unsigned long*) *(sp+2);
  printk("esp: %lx, sp: %p, ret: %lx, a_info: %p, a_var: %p, eip: %lx, ebp: %lx\n",
  	regs->esp, sp, ret, a_info, a_var, regs->eip, regs->ebp);

  for (i = 0; i < 10; i++) {
  	printk("%lx\n", sp[i]);
  }

  return 0;
}*/

static __init int init_myjp(void)
{
  printk("Hello\n");

  //
  // Settting up kprobes;
  //
  jp.kp.symbol_name = "drm_ioctl";
  jp.entry= JPROBE_ENTRY(jp_drm_ioctl);
  register_jprobe(&jp);

  /*
  jp0.kp.symbol_name = "fb_ioctl";
  jp0.entry= JPROBE_ENTRY(jp_fb_ioctl);
  register_jprobe(&jp0);
  */

  return 0;
}
module_init(init_myjp);

static __exit void cleanup_myjp(void)
{
  unregister_jprobe(&jp);
  //unregister_jprobe(&jp0);
  printk("Good by!\n");
}
module_exit(cleanup_myjp);

MODULE_LICENSE("GPL");
