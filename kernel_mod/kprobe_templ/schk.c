#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/kallsyms.h>
#include <linux/fb.h>

struct jprobe jp0;
struct jprobe jp;

int
jp_fb_set_var(struct fb_info *info, struct fb_var_screeninfo *var)
//jp_fb_set_var(void *info, void *var)
{
	printk("info: %p, fb_var_screeninfo: %p, x: %d, y: %d,\n",
		info, var, var->xres, var->yres);
	jprobe_return();
	return 0;
}

int
jp_fb_ioctl(struct inode *inode, struct file *file, unsigned int cmd,
	unsigned long arg) 
{
	printk("cmd: 0x%x\n", cmd);
	jprobe_return();
	return 0;
}

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

static __init int init_schk(void)
{
  printk("Hello\n");

  //
  // Settting up kprobes;
  //
  jp.kp.symbol_name = "fb_set_var";
  jp.entry= JPROBE_ENTRY(jp_fb_set_var);
  register_jprobe(&jp);

  jp0.kp.symbol_name = "fb_ioctl";
  jp0.entry= JPROBE_ENTRY(jp_fb_ioctl);
  register_jprobe(&jp0);

  return 0;
}
module_init(init_schk);

static __exit void cleanup_schk(void)
{
  unregister_jprobe(&jp);
  unregister_jprobe(&jp0);
  printk("Good by!\n");
}
module_exit(cleanup_schk);

MODULE_LICENSE("GPL");
