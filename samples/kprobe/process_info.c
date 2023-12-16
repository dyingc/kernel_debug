#include "process_info.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eric Dong");
MODULE_DESCRIPTION("Provide the functionality to get the current process' information");

struct process_info get_process_info(void) {
    struct process_info info;
    struct mm_struct *mm = current->mm;
    unsigned long arg_start, arg_end, len;

    info.pid = current->pid;
    info.comm = kstrdup(current->comm, GFP_KERNEL);

    if (!mm || !info.comm) {
        info.cmdline = NULL;
        return info;
    }

#ifdef CONFIG_MMU
    if (down_read_trylock(&mm->mmap_lock) == 0) // Updated to use mmap_lock
        return info;
#else
    if (down_read_trylock(&mm->mmap_sem) == 0) // Fallback to mmap_sem for non-MMU systems
        return info;
#endif

    arg_start = mm->arg_start;
    arg_end = mm->arg_end;

    if (arg_end <= arg_start) {
        info.cmdline = NULL;
        goto out_unlock;
    }

    len = arg_end - arg_start;
    info.cmdline = kmalloc(len, GFP_KERNEL);

    if (!info.cmdline)
        goto out_unlock;

    info.cmdline[len - 1] = '\0';

    if (copy_from_user(info.cmdline, (char __user *)arg_start, len)) {
        kfree(info.cmdline);
        info.cmdline = NULL;
        goto out_unlock;
    }

out_unlock:
#ifdef CONFIG_MMU
    up_read(&mm->mmap_lock);
#else
    up_read(&mm->mmap_sem);
#endif

    return info;
}

EXPORT_SYMBOL_GPL(get_process_info);
