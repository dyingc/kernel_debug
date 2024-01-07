# not to optimize the code unless it's debug-non-related, to make debug easier
ccflags-y := -Og
obj-m := hello.o
obj-m += kb.o

# Kernel source directory
KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules
debug: all
	scp *.c *.ko dev:$(PWD)/
	sudo insmod hello.ko
	sudo insmod kb.ko
	sudo cat /sys/module/hello/sections/.text
clean:
	make -C $(KDIR) M=$(PWD) clean
debug_clean: clean
	sudo rmmod hello 2>/dev/null
	sudo rmmod kb 2>/dev/null
