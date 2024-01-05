# Debugging Kernel Modules

Kernel Modules are separate object files that can be loaded into the kernel's address space at runtime.

Each kernel module contains its own symbols that are separate from the kernel's symbols. 

Loadable kernel module is divided up into several sections.

- `.text` - contains the module code
- `.data` - and .bss contain most of the variables

Kgdb needs to be informed about this symbols using `add-symbol-file` command.

This command requires the relocated addresses of each section as command arguments.

```bash
(gdb) add-symbol-file /path/to/module <addr> \ #.text
	-s .data <addr> \ # optional
	-s .bss <addr>    # optional
```

The **<addr>** can be found from `/sys/module/<module>/sections/.text|.data|.bss`

```bash
root@ubuntu19:/sys/module/e1000/sections# cat .text
0xffffffffc0036000
root@ubuntu19:/sys/module/e1000/sections# cat .bss
0xffffffffc0053800
root@ubuntu19:/sys/module/e1000/sections# cat .data
0xffffffffc0051000
```


# Demo: Debugging e1000 module

On the target VM
------------------

Transfer the kernel build folder (including: source and all the binaries, eg: `/lib/modules/$(uname -r)/build`. For this course, it's already copied to `DevVM:~/udemy/kernel/kgdb/<version>`) to the Dev VM so that those in-tree kernel modules and their source code will be the same from the Dev VM and the Target VM. Note, this is only needed if you want to debug the in-tree KM, or simply `vmlinux` is sufficient.

Find the to-be-debug module:

```
dyingc@ubuntu19:~$ locate e1000.ko | grep "`uname -r`"
/usr/lib/modules/5.4.30-kernel-debug/kernel/drivers/net/ethernet/intel/e1000/e1000.ko
```

gdb client needs to know the load address of the kernel module

```bash
$ sudo cat /sys/module/e1000/sections/.text
0xffffffffc0036000
```

Switch to KGDB Debugger mode by activating magic sysrq

```bash
$ echo 'ttyS0,115200' | sudo tee /sys/module/kgdboc/parameters/kgdboc
$ echo | sudo tee > /proc/sysrq-trigger
```

On the Development VM
-----------------------

Run the following commands:

```bash
$ sudo gdb ./vmlinux
(gdb) target remote /dev/ttyS1
(gdb) add-symbol-file drivers/net/ethernet/intel/e1000/e1000.ko 0xffffffffc0036000
```

Let's put breakpoint at `e1000_xmit_frame` which is called while transmitting a frame.

```bash
(gdb) break e1000_xmit_frame
Breakpoint 1 at 0xffffffffc0092f90: file drivers/net/ethernet/intel/e1000/e1000_main.c, line 3102.
(gdb) c
```

Now see as soon as it transmits a packet, system will stop and enter into kgdb session
