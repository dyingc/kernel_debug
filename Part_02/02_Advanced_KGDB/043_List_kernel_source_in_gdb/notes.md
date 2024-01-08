# Using gdb to see the kernel source listing

You can use the addresses printed out in kdb, with a host-side gdb session, to see the source code or assembly instructions around a particular address.

The target address can come from a backtrace or register dump (e.g. instruction pointer).

Use the following commands to see various bits of information:

- source file and line number for an instruction address

```bash
(gdb) info line *0x<target_addr>
```
- source lines around an instruction address

```bash
(gdb) list *0x<target_addr>
```

- assembly instructions at an address

```bash
(gdb) disas 0x<target_addr>, or
(gdb) x/20i 0x<target_addr>
```
