#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/in.h>     // Include for struct sockaddr_in
#include <linux/in6.h>    // Include for struct sockaddr_in6

#include <linux/sched/signal.h>  // for for_each_process - to get full command line
#include <linux/mm_types.h>      // for mm_struct - to get full command line
#include <linux/fs.h>            // for file and path structures - to get full command line

/**
 * Note, this one add pre and post hook to system call "bind".
 * This will generate huge amount of output by the `printk` function.
 * You can cleanup the `/var/log/kern.log` after testing (sudo rmmod kprobe_read)
 * using: echo -n | sudo tee /var/log/kern.log
 **/

static struct kprobe kp;

static int handler_pre(struct kprobe *p, struct pt_regs *regs) { // the `pt_regs` is defined in: /usr/src/linux-headers-5.0.0-38/arch/x86/include/asm/ptrace.h
    unsigned long sockfd = regs->di; // First argument - Socket file descriptor
    struct sockaddr __user *addr = (struct sockaddr __user *)regs->si; // Second argument - Socket address - a pointer to `struct sockaddr` in user space
    unsigned long addrlen = regs->dx; // Third argument - Length of address
    struct sockaddr_storage address;
    unsigned short port;
    // The following are used for fetch full command line
    struct mm_struct *mm = current->mm;
    // struct vm_area_struct *vma;
    char *cmdline;
    unsigned long arg_start, arg_end, len;

    printk(KERN_INFO "Before copy_from_user: sockfd=%lu, addr=%px, addrlen=%lu\n", regs->di, addr, regs->dx);

    // Safely copy the address from user space
    if (copy_from_user(&address, addr, addrlen)) {
        printk(KERN_INFO "Error copying sockaddr from user space\n");
        return 0;
    } else {
	printk(KERN_INFO "%lu bytes of data have been copied from user space\n", addrlen);
    }

    // Handle IPv4 and IPv6 addresses
    if (address.ss_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&address;
        port = ntohs(addr_in->sin_port);
        printk(KERN_INFO "Binding IPv4 (sockfd: %lu): %pI4:%u\n", sockfd, &addr_in->sin_addr.s_addr, port);
    } else if (address.ss_family == AF_INET6) {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&address;
        port = ntohs(addr_in6->sin6_port);
        printk(KERN_INFO "Binding IPv6 (sockfd: %lu): %pI6:%u\n", sockfd, &addr_in6->sin6_addr, port);
    } else {
        printk(KERN_INFO "Unsupported socket family: address.ss_family = %d\n", address.ss_family);
    }

    // Get PID and command of the calling process
    printk(KERN_INFO "PID: %d, Command: %s\n", current->pid, current->comm);

    // Getting full command line arguments is complex and might require additional code
    if (!mm)
        goto out;

    // Ensure safe memory access
    if (down_read_trylock(&mm->mmap_sem) == 0)
        goto out;

    arg_start = mm->arg_start;
    arg_end = mm->arg_end;

    if (arg_end <= arg_start)
        goto out_unlock;

    len = arg_end - arg_start;

    // Allocate memory to store command line arguments
    cmdline = kmalloc(len, GFP_KERNEL);
    if (!cmdline)
        goto out_unlock;

    if (copy_from_user(cmdline, (char __user *)arg_start, len)) {
        printk(KERN_INFO "Failed to copy command line arguments\n");
        kfree(cmdline);
        goto out_unlock;
    }

    // Null-terminate and print the command line
    cmdline[len - 1] = '\0';
    printk(KERN_INFO "Command Line: %s\n", cmdline);

    kfree(cmdline);

out_unlock:
    up_read(&mm->mmap_sem);
out:

    return 0;
}

static void handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
    printk(KERN_INFO "Post-handler: Probed function completed\n");
    // Access registers using regs pointer if needed
}

static int __init kprobe_init(void) {
    int ret;
    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;
    // kp.symbol_name = "__x64_sys_bind"; // Use "__ia32_sys_bind" for 32-bit systems
    kp.symbol_name = "__sys_bind"; // Use "__ia32_sys_bind" for 32-bit systems

    ret = register_kprobe(&kp);
    printk(KERN_INFO "Register return code: %d\n", ret);
    if (ret < 0) {
        printk(KERN_INFO "Register kprobe failed, returned %d\n", ret);
        return -1;
    }
    printk(KERN_INFO "Planted kprobe at %p, handler addr %p\n", kp.addr, kp.pre_handler);
    return 0;
}

static void __exit kprobe_exit(void) {
    unregister_kprobe(&kp);
    printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
}

module_init(kprobe_init)
module_exit(kprobe_exit)

MODULE_LICENSE("GPL");
