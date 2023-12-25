# Object files to be built
obj-m += process_info.o kprobe_bind.o

# Kernel source directory
KDIR := /lib/modules/$(shell uname -r)/build

EXTRA_CFLAGS :=

all:
	# ./prepare_kernel.sh
	make -C $(KDIR) M=$(PWD) modules EXTRA_CFLAGS='$(EXTRA_CFLAGS)'
# Enable debug symbols
debug: EXTRA_CFLAGS := -g -O0
debug: all
	sudo insmod process_info.ko 
	sudo insmod kprobe_bind.ko
	sudo cat /sys/module/process_info/sections/.text /sys/module/kprobe_bind/sections/.text
	scp *.c *.h *.ko dev:$(PWD)
clean:
	make -C $(KDIR) M=$(PWD) clean

debug_clean: clean
	rm -f bind_test 2>/dev/null
	sudo rmmod kprobe_bind process_info 2>/dev/null

bind_test:
	gcc -o bind_test bind_test.c
