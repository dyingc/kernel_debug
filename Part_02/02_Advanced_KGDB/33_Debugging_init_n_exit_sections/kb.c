#include <linux/module.h>    // Essential for all kernel modules
#include <linux/init.h>      // Needed for the macros
#include <linux/kernel.h>    // Needed for KERN_INFO

MODULE_LICENSE("GPL"); // Specifying the license type for the module.
 
static int kb_init(void) {
        // Create a directory in the proc file system
        pr_info("ldd proc entry created\n"); // Log the creation of the proc file
        return 0; // Return success
}

static void kb_cleanup(void) {
        pr_info("ldd proc entry deleted\n"); // Log the creation of the proc file
}

module_init(kb_init);   // Register the initialization function
module_exit(kb_cleanup); // Register the cleanup function

