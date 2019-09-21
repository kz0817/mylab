obj-m := schk.o devmem_ret_one.o \
    kp_alter_ovl_tnc.o \
    kp_igd_alter_ovl2.o \
    kp_emgd_alter_ovl2.o \
    jp_drm_ioctl.o \
    kp_trace_many.o \
    kretp_alter_ovl_tnc.o \
    kp_drm_wait_vblank.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` V=1

.PHONY: clean ins rm rei
clean:
	rm -fr *.ko *.o *.mod.* Module.symvers

ins:
	insmod schk.ko

rm:
	rmmod schk.ko

rei:
	rmmod schk.ko
	insmod schk.ko
