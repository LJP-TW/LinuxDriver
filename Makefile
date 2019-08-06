CONFIG_MODULE_SIG=n
obj-m += ljp.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	-rm output.txt

ljp.ko:all

test:ljp.ko
	-sudo rmmod ljp
	sudo dmesg -C
	sudo insmod ljp.ko
	dmesg
	# You should create a device by typing command below:
	# sudo mknod /dev/ljp c MAJOR_num 0
	# (dmesg show the MAJOR_num, plz replace it)
	# 
	# Then, you can use `cat /dev/ljp`, see what happens :)
	# or just use `dd` below:
	# dd if=/dev/ljp of=output.txt bs=32 count=1
	#
	# If you wanna remove this device, just use `rm`:
	# sudo rm /dev/ljp
