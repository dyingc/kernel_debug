# Set breakpoint on condition

If you are going to set a breakpoint in `vfs_open`, it will be triggered thousands of times.

If you want to trigger `vfs_open` only for a particular file, you can use gdb convenience functions

$_streq(str1, str2) : Returns one if the strings str1 and str2 are equal. Otherwise it returns zero

(gdb) b vfs_open if $_streq(path.dentry->d_iname, "lwl")
