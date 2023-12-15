#obj-m += kprobe_read.o
obj-m += kprobe_bind.o

all:
	./prepare_kernel.sh
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	sudo insmod kprobe_bind.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f bind_test
	sudo rmmod kprobe_bind

bind_test:
	gcc -o bind_test bind_test.c
