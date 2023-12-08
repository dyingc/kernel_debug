#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

// Prototype of the original 'read' system call
ssize_t (*original_read)(int fd, void *buf, size_t count);

// Jprobe handler function
static long jprobe_read(int fd, void *buf, size_t count) {
    struct file *file;
    char *tmp = (char *)__get_free_page(GFP_KERNEL);
    char *pathname;
    pid_t pid;

    if (!tmp)
        return -ENOMEM;

    // Get current process ID
    pid = current->pid;
    printk(KERN_INFO "Read Hook: Process ID: %d\n", pid);

    // Get filename from file descriptor
    file = fget(fd);
    if (file) {
        pathname = d_path(&file->f_path, tmp, PAGE_SIZE);
        if (!IS_ERR(pathname)) {
            printk(KERN_INFO "Read Hook - by Eric: File to read: %s\n", pathname);
        }
        fput(file);
    }

    free_page((unsigned long)tmp);

    // Call the original 'read' system call
    jprobe_return();
    return original_read(fd, buf, count);
}

static struct jprobe my_jprobe = {
    .entry = jprobe_read,
    .kp = {
        .symbol_name = "sys_read",
    },
};

// Module initialization function
static int __init jprobe_init(void) {
    int ret;

    ret = register_jprobe(&my_jprobe);
    if (ret < 0) {
        printk(KERN_INFO "Register jprobe failed, returned %d\n", ret);
        return -1;
    }
    printk(KERN_INFO "Planted jprobe at %p, handler addr %p\n",
           my_jprobe.kp.addr, my_jprobe.entry);
    return 0;
}

// Module cleanup function
static void __exit jprobe_exit(void) {
    unregister_jprobe(&my_jprobe);
    printk(KERN_INFO "jprobe at %p unregistered\n", my_jprobe.kp.addr);
}

module_init(jprobe_init)
module_exit(jprobe_exit)

MODULE_LICENSE("GPL");

