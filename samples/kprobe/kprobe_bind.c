#include "process_info.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/in.h>     // Include for struct sockaddr_in
#include <linux/in6.h>    // Include for struct sockaddr_in6

#include <linux/sched/signal.h>  // for for_each_process - to get full command line
#include <linux/mm_types.h>      // for mm_struct - to get full command line
#include <linux/fs.h>            // for file and path structures - to get full command line
#include <linux/kgdb.h>         // for kgdb_breakpoint

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Eric Dong");
MODULE_DESCRIPTION("Monitor the __sys_bind system call");

char *mon_app_name = "test_bind";
module_param(mon_app_name, charp, S_IRUGO); // Makes 'mon_app_name' a read-only module parameter for userspace.

/**
 * Note, this one add pre and post hook to system call "bind".
 * This will generate huge amount of output by the `printk` function.
 * You can cleanup the `/var/log/kern.log` after testing (sudo rmmod kprobe_read)
 * using: echo -n | sudo tee /var/log/kern.log
 **/

static struct kprobe kp;
static char* symbol_name = "__sys_bind"; // It's confirmed that we have to use this name rather than "__x64_sys_bind". Consider to use "__ia32_sys_bind" for 32-bit systems

static int handler_pre(struct kprobe *p, struct pt_regs *regs) { // the `pt_regs` is defined in: /usr/src/linux-headers-5.0.0-38/arch/x86/include/asm/ptrace.h
    unsigned long sockfd = regs->di; // First argument - Socket file descriptor
    struct sockaddr __user *addr = (struct sockaddr __user *)regs->si; // Second argument - Socket address - a pointer to `struct sockaddr` in user space
    unsigned long addrlen = regs->dx; // Third argument - Length of address
    struct sockaddr_storage address;
    unsigned short port;
    struct process_info p_info = get_process_info();

        pr_info("%s (%d): mon_app_name = \"%s\", p_info.comm = \"%s\"\n", __FUNCTION__, __LINE__, mon_app_name, p_info.comm);
    if (strcmp(p_info.comm, mon_app_name) != 0) // Skip the following code if the current process is not the one we're interested in
        goto out;

    // kgdb_breakpoint();

    //printk(KERN_INFO "Before %s: sockfd=%lu, addr=%px, addrlen=%lu\n", symbol_name, regs->di, addr, regs->dx);

    // Safely copy the address from user space
    if (copy_from_user(&address, addr, addrlen)) {
        printk(KERN_INFO "Error copying sockaddr from user space\n");
        return 0;
    } else {
    // printk(KERN_INFO "%lu bytes of data have been copied from user space\n", addrlen);
    }

    // Handle IPv4 and IPv6 addresses
    if (address.ss_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&address;
        port = ntohs(addr_in->sin_port);
        /**
         * You can set breakpoint here and output anything you want.
         * However, in order not to trigger Oops, you have to:
         * 1. delete the breakpoint
         * 2. Do NOT use "n" or "s", rather, you should use "c" to directly continue
         * The reason for this limitation might due to the fact that a lot of system-level
         * applications will trigger this function, including "sudo",
         * "systemd-udevd", "sshd", "cups-browsed", etc
        **/
        printk(KERN_INFO "Binding IPv4 (sockfd: %lu): %pI4:%u - PID: %d, Command: %s (%s)\n", sockfd, &addr_in->sin_addr.s_addr, port, p_info.pid, p_info.comm, p_info.cmdline);
    } else if (address.ss_family == AF_INET6) {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&address;
        port = ntohs(addr_in6->sin6_port);
        printk(KERN_INFO "Binding IPv6 (sockfd: %lu): %pI6:%u - PID: %d, Command: %s (%s)\n", sockfd, &addr_in6->sin6_addr, port, p_info.pid, p_info.comm, p_info.cmdline);
    } else if (address.ss_family == AF_NETLINK) { // Communication between kernel and userspace applications
        //printk(KERN_INFO "Network Link: address.ss_family = %d\n", address.ss_family);
        goto out;
    } else {
        printk(KERN_INFO "Unsupported socket family: address.ss_family = %d\n", address.ss_family);
    }

out:
    kfree(p_info.comm);
    kfree(p_info.cmdline);
    return 0;
}

static void handler_post(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
    //printk(KERN_INFO "Post-handler: Probed function completed\n");
    // Access registers using regs pointer if needed
}

// We need to remove the "__init" direct to enable the "init" func debug
#ifdef DEBUG_INIT
static int kprobe_init(void) {
#else
static int __init kprobe_init(void) {
#endif
    int ret;
    kp.pre_handler = handler_pre;
    kp.post_handler = handler_post;
    kp.symbol_name = symbol_name;

    ret = register_kprobe(&kp);
    printk(KERN_INFO "Register return code: %d\n", ret);
    if (ret < 0) {
        printk(KERN_INFO "Register kprobe failed, returned %d\n", ret);
        return -1;
    }
    printk(KERN_INFO "Planted kprobe at %p, handler addr %p\n", kp.addr, kp.pre_handler);
    return 0;
}

#ifdef DEBUG_EXIT
static void kprobe_exit(void) {
#else
static void __exit kprobe_exit(void) {
#endif
    unregister_kprobe(&kp);
    printk(KERN_INFO "kprobe at %p unregistered\n", kp.addr);
    //kgdb_breakpoint();
}

module_init(kprobe_init)
module_exit(kprobe_exit)
