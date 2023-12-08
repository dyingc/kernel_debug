#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>

/**
 * Note, this one add pre and post hook to system call "read".
 * This will generate huge amount of output by the `printk` function.
 * You can cleanup the `/var/log/kern.log` after testing (sudo rmmod kprobe_read)
 * using: echo -n | sudo tee /var/log/kern.log
 **/

static struct kprobe kp;

static int handler_pre(struct kprobe *p, struct pt_regs *regs) {
    printk(KERN_INFO "Pre-handler: Probed function called\n");
    // Access registers using regs pointer if needed
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
    kp.symbol_name = "__x64_sys_read"; // Use "__ia32_sys_read" for 32-bit systems

    ret = register_kprobe(&kp);
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
