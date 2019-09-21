obj-m := mod_ex.o

all:
	make -C /lib/modules/`uname -r`/build M=`pwd` V=1

.PHONY: clean
clean:
	rm -fr *.ko *.o *.mod.* Module.symvers
