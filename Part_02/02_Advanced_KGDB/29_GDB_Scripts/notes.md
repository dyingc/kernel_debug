# Using Linux Provided GDB Scripts

Linux kernel has provided a collection of helper scripts which can simplify kernel debugging steps.


## Enable

Ensure the `GDB_SCRIPTS` is enabled on the Target VM:

```bash
dyingc@ubuntu19:/lib/modules/5.4.30-kernel-debug/build$ cat /boot/config-$(uname -r) | grep CONFIG_GDB_SCRIPTS
CONFIG_GDB_SCRIPTS=y
```

You can observe in the linux source directory a file named `vmlinux-gdb.py`. To use this, copy the below command in Dev VM's `~/.gdbinit`

```bash
add-auto-load-safe-path /path/to/copied/linux/build
```

```bash
dyingc@kernel-dev:~/udemy/kernel/kgdb/linux-5.4.0$ cat /root/.gdbinit # Because I use `sudo gdb vmlinux` so I need to change root's config

# warning: File "/home/dyingc/udemy/kernel/kgdb/linux-5.0/vmlinux-gdb.py" auto-loading has been declined by your `auto-load safe-path' set to "$debugdir:$datadir/auto-load".
# To enable execution of this file add the following line to your configuration file "~/.gdbinit":

# add-auto-load-safe-path /home/dyingc/udemy/kernel/kgdb/linux-5.0

add-auto-load-safe-path /home/dyingc/udemy/kernel/kgdb/linux-5.4.0

# To completely disable this security protection add the following line to your configuration file "~/.gdbinit":

set auto-load safe-path /

# For more information about this security protection see the
# "Auto-loading safe path" section in the GDB manual.  E.g., run from the shell:
#        info "(gdb)Auto-loading safe path"
```

List of commands supported by the script can be found by running:

(gdb) apropos lx

Let's try some of the commands.

1. To find out the kernel command line with which the target booted. "lx-cmdline"
```bash
(gdb) lx-cmdline
BOOT_IMAGE=/boot/vmlinuz-5.4.30-kernel-debug root=/dev/mapper/ubuntu--vg-root ro quiet splash nokaslr console=ttyS0,115200 kgdboc=ttyS0,115200
```

2. To find out the list of processes . "lx-ps"
```bash
(gdb) lx-ps                      
0xffffffff82613780 <init_task> 0 swapper/0
0xffff8887f6b12e80 1 systemd                                    
0xffff8887f6b145c0 2 kthreadd
0xffff8887f6b10000 3 rcu_gp
0xffff8887f6b11740 4 rcu_par_gp
0xffff8887f6b35d00 6 kworker/0:0H
0xffff8887f6b32e80 7 kworker/0:1
0xffff8887f6b30000 9 mm_percpu_wq
0xffff8887f6b31740 10 ksoftirqd/0
... ... ... ...
0xffff8887ef012e80 3058 gdbus
0xffff8887ef0145c0 3059 threaded-ml
0xffff8887a5915d00 3088 kworker/u36:0
0xffff8887a5910000 3089 kworker/6:1
0xffff88878ed90000 3090 kworker/u36:3
0xffff8887d56c1740 3091 enable_debug.sh
0xffff8887d74845c0 3096 sudo
0xffff8887e091dd00 3097 tee
(gdb) 
```

3. To find out list of loaded modules: "lx-lsmod"
```bash
(gdb) lx-lsmod                                    
Address            Module                  Size  Used by
0xffffffffc03ac000 binfmt_misc            24576  1                
0xffffffffc039a000 snd_intel8x0           45056  2
0xffffffffc0370000 snd_ac97_codec        131072  1 snd_intel8x0
0xffffffffc0368000 ac97_bus               16384  1 snd_ac97_codec
0xffffffffc0342000 snd_pcm               106496  2 snd_intel8x0,snd_ac97_codec
0xffffffffc0321000 snd_seq_midi           20480  0
0xffffffffc025a000 vboxvideo              32768  2
0xffffffffc0243000 snd_seq_midi_event     16384  1 snd_seq_midi
... ... ... ...
0xffffffffc000e000 libahci                32768  1 ahci
0xffffffffc0002000 i2c_piix4              28672  0
(gdb) 
```

4. To find out the version of the current kernel  -  "lx-version"
```bash
(gdb) lx-version
Linux version 5.4.30-kernel-debug (root@ubuntu19) (gcc version 9.4.0 (Ubuntu 9.4.0-1ubuntu1~20.04.2)) #1 SMP Sat Dec 23 19:17:17 PST 2023 (Ubuntu 5.4.0-169.187-generic 5.4.257)
(gdb) 
```
(gdb) help <command>
