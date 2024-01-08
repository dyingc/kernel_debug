# Saving and Restoring Breakpoints using Files

You have spent lot of time finding and adding breakpoints.

After few minutes of debugging, due to some unknown error your gdb session quits

When you start fresh with a new session, you need to add those breakpoints again

Solution: 

- Saving breakpoints to a file

```bash
(gdb) save breakpoints mybreakpt.brk
```

This will save all types of breakpoints (regular breakpoints, watchpoints and catchpoints) to the file

- Load the saved breakpoints

Later when you're ready to reload your breakpoints, you can issue the source command

```bash
(gdb) source mybreakpt.brk
```
