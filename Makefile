CONFIG_MODULE_SIG=n
obj-m += ljp.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	-rm output.txt
	-sudo rmmod ljp

ljp.ko:all

test:ljp.ko
	-sudo rmmod ljp
	sudo dmesg -C
	sudo insmod ljp.ko
	dmesg
	dd if=/dev/ljp_dev of=output.txt bs=16 count=1

observe:
	-cat /proc/devices | grep ljp_proc
	-ls -al /sys/class | grep ljp_sys
	-ls -al /dev | grep ljp_dev
