obj-m += hello.o

all: client
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules
	chmod +x ./insmod.sh; sudo ./insmod.sh
client:
	gcc -o userapp userapp.c
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	chmod +x ./rmmod.sh; sudo ./rmmod.sh
