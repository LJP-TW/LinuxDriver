CONFIG_MODULE_SIG=n
obj-m += ljp.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	-rm output.txt
	-rm test.txt
	-rm test
	-sudo rmmod ljp

ljp.ko:all

load:ljp.ko
	-sudo rmmod ljp
	sudo dmesg -C
	sudo insmod ljp.ko
	dmesg

test_dd:load
	dd if=/dev/ljp_dev of=test.txt bs=16 count=1
	cat test.txt

test_program:load
	gcc test.c -o test
	./test

observe:
	-cat /proc/devices | grep ljp_proc
	-ls -al /sys/class | grep ljp_sys
	-ls -al /dev | grep ljp_dev
