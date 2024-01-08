# SPDX-License-Identifier: GPL-2.0-only
# builds the kprobes example kernel modules;
# then to use one (as root):  insmod <module_name.ko>

obj-$(CONFIG_SAMPLE_KPROBES) += kprobe_example.o
obj-$(CONFIG_SAMPLE_KRETPROBES) += kretprobe_example.o

obj-m := kprobe_example.o
obj-m += kretprobe_example.o

# not to optimize the code at all and enable debug symbols
ccflags-y := -g -O0

kernel-headers := '/lib/modules/$(shell uname -r)/build'

all:
	make -C $(kernel-headers) M=$(PWD) modules

clean:
	make -C $(kernel-headers) M=$(PWD) clean

