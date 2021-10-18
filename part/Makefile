DEVICE ?= drive.img

_step1: drive.img
	parted $(DEVICE) mklabel msdos
	parted $(DEVICE) mkpart primay 1M 10M
	parted $(DEVICE) mkpart primay 10M 30M
	parted $(DEVICE) mkpart extended 30M 204799s
	parted $(DEVICE) mkpart logical 61440s 88063s
	parted $(DEVICE) mkpart logical 90112s 176127s
	parted $(DEVICE) mkpart logical 178176s 99M

_step2: drive.img
	parted -s $(DEVICE) mklabel gpt
	parted $(DEVICE) mkpart primay 1M 10M
	parted $(DEVICE) mkpart primay 10M 30M
	parted $(DEVICE) mkpart primary 61440s 88063s
	parted $(DEVICE) mkpart primary 90112s 176127s
	parted $(DEVICE) mkpart primary 178176s 99M

drive.img:
	dd if=/dev/zero of=$@ bs=1M count=100
