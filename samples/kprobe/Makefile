# There can be at most two "_" in the `km_name`, or it can't be inserted (incorrect km format)
km_name := mon_bind
obj-m += $(km_name).o
$(km_name)-objs := process_info.o kprobe_bind.o

kernel_headers := /lib/modules/$(shell uname -r)/build

# Enable debug symbols
# ccflags-y := -g

# not to optimize the code unless it's debug-non-related, to make debug easier
# ccflags-y += -Og
# not to optimize the code at all and enable debug symbols
ccflags-y := -g -O0

# For debug init or exit function, use: `make DEBUG_INIT=1`, else, use: `make`
ifdef DEBUG_INIT
ccflags-y += -DDEBUG_INIT
ccflags-y += -DDEBUG_EXIT
endif

# EXTRA_CFLAGS is a more general option while `ccflags-y` is typically used for KM
# EXTRA_CFLAGS += -Og

all:
	#./prepare_kernel.sh
	make -C $(kernel_headers) M=$(PWD) modules
	#sudo insmod $(km_name).ko
debug: all
	ssh dev "rm -f $(PWD)/*"; scp *.h *.c *.ko dev:$(PWD)/
clean:
	make -C $(kernel_headers) M=$(PWD) clean
	rm -f bind_test 2>/dev/null
	#sudo rmmod $(km_name) 2>/dev/null

bind_test:
	gcc -o bind_test bind_test.c
