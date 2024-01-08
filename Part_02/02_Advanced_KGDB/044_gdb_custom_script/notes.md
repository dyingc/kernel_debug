# Pass commands to `gdb` - Use GDB custom script

You can pass commands to gdb on the command line with option -ex.

--eval-command=COMMAND, -ex
	Execute a single GDB command.
	May be used multiple times and in conjunction with --command.


## Example 1

```bash
$ sudo gdb ./vmlinux
(gdb) target remote /dev/ttyS0
(gdb) break cmdline_proc_show
(gdb) continue
```

or

```bash
$ sudo gdb -ex 'target remote /dev/ttyS0' -ex 'break cmdline_proc_show' -ex 'continue' ./vmlinux
```


## Example 2

Useful with scripts:

sudo gdb `-x <gdb_script>` vmlinux

```bash
dyingc@kernel-dev:~/udemy/kernel/kgdb/linux-5.4.0$ cat kgdb_kernel.txt 
set logging file /tmp/kernel.log
set logging overwrite on
set print pretty
target remote localhost:5550
c
dyingc@kernel-dev:~/udemy/kernel/kgdb/linux-5.4.0$ sudo gdb -x kgdb_kernel.txt vmlinux
GNU gdb (Ubuntu 8.2.91.20190405-0ubuntu3) 8.2.91.20190405-git
Copyright (C) 2019 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from vmlinux...
kgdb_breakpoint () at kernel/debug/debug_core.c:1124
1124            wmb(); /* Sync point after breakpoint */
```
