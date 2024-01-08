# Temporary Breakpoints

A temporary breakpoint only stops the execution of the program once.

When the program hits a temporary breakpoint, it deletes automatically, which saves you the trouble of managing and cleaning up obsolete breakpoints

You can set a temporary breakpoint with the following command:

tbreak [LOCATION]

```bash
(gdb) tbreak cmdline_proc_show
```




