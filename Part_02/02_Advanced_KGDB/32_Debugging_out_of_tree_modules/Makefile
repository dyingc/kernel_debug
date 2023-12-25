# Enable debug symbols
ccflags-y := -g
obj-m := hello.o

# Kernel source directory
KDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules
debug: all
	sudo insmod hello.ko
	sudo cat /sys/module/hello/sections/.text
	scp *.c *.ko dev:$(PWD)/
clean:
	make -C $(KDIR) M=$(PWD) clean
debug_clean: clean
	sudo rmmod hello 2>/dev/null
