# rbreak - Batch set breakpoint using regexp

With "break" command, you can set one breakpoint at a time.

Consider a scenario where you want to set a lot of breakpoints.

Solution: rbreak or rb

The gdb "rbreak" command allows you to use grep-style regular expressions to set breakpoint locations.

**Syntax:** `rbreak REGEXP`

- Set breakpoints for all functions starting with `vfs_`

```bash
(gdb) rbreak ^vfs_.*
```

- To delete all breakpoints

```bash
(gdb) delete breakpoints
```
or
```bash
(gdb) del
```

- Set breakpoints for all functions defined in `fs/read_write.c` and starting with `vfs_`

```bash
(gdb) rbreak fs/read_write.c:^vfs_.*
```

- What if i want to set breakpoints for every function in a given file?

```bash
(gdb) rbreak fs/read_write.c:.*
```

