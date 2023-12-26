#include <linux/module.h>      // Necessary for any Linux kernel module.
#include <linux/init.h>        // For module initialization and cleanup functions.
#include <linux/kernel.h>      // Contains types, macros, functions for the kernel.
#include <linux/proc_fs.h>     // For proc filesystem operations.
#include <asm/uaccess.h>       // For user space memory access functions.
 
MODULE_LICENSE("GPL");          // Declares the license of the module (GNU Public License).

static struct proc_dir_entry *dir_entry;  // Pointer to a proc directory entry.

// Write operation for the proc file.
static ssize_t mywrite(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) {
  pr_info("write handler\n");  // Logging statement for write operations.
  panic("hello");              // A panic call (commented out) which would crash the kernel.
  return count;                // Returns the number of bytes written.
}

// Read operation for the proc file.
static ssize_t myread(struct file *file, char __user *ubuf, size_t count, loff_t *ppos) {
  pr_info("read handler\n");   // Logging statement for read operations.
  return 0;                     // Returns 0, indicating no data read.
}

// File operations structure for the proc file.
static struct file_operations proc_ops = {
  .owner = THIS_MODULE,         // Macro to auto-fill the owner field with the current module.
  .read = myread,               // Assigns the read operation.
  .write = mywrite,             // Assigns the write operation.
};

// Initialization function for this module.
static int proc_init(void) {
  // Create a proc entry named "ldd".
  dir_entry = proc_create("ldd", 0640, NULL, &proc_ops);
  if (!dir_entry) {
    pr_err("%s: error creating proc_entry\n", __func__);  // Log an error if creation fails.
    return -1; 
  }
  pr_info("ldd proc entry created\n");   // Log success message.
  return 0;                              // Return 0 on successful initialization.
}

// Cleanup function for this module.
static void proc_cleanup(void) {
  remove_proc_entry("ldd", NULL);        // Removes the proc entry upon module cleanup.
}

module_init(proc_init);                  // Macros to specify module entry and exit points.
module_exit(proc_cleanup);

