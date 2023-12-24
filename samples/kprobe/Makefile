# Enable debug symbols
ccflags-y := -g

# Object files to be built
obj-m += process_info.o kprobe_bind.o

all:
	./prepare_kernel.sh
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod process_info.ko 
	sudo insmod kprobe_bind.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f bind_test 2>/dev/null
	sudo rmmod kprobe_bind process_info 2>/dev/null

bind_test:
	gcc -o bind_test bind_test.c
