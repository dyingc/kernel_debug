# There can be at most two "_" in the `km_name`, or it can't be inserted (incorrect km format)
km_name := mon_bind
obj-m += $(km_name).o
$(km_name)-objs := process_info.o kprobe_bind.o

kernel_headers := /lib/modules/$(shell uname -r)/build
all:
	#./prepare_kernel.sh
	make -C $(kernel_headers) M=$(PWD) modules
	sudo insmod $(km_name).ko

clean:
	make -C $(kernel_headers) M=$(PWD) clean
	rm -f bind_test 2>/dev/null
	sudo rmmod $(km_name) 2>/dev/null

bind_test:
	gcc -o bind_test bind_test.c
