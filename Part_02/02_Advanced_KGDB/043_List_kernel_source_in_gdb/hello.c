#include <linux/module.h>    // Essential for all kernel modules
#include <linux/init.h>      // Needed for the macros
#include <linux/kernel.h>    // Needed for KERN_INFO
#include <linux/proc_fs.h>   // Interface for the proc file system
#include <linux/uaccess.h>   // For copy_from_user and copy_to_user functions

 
char buffer[] = "Hello Linux";   // The buffer to hold the string
#define MAX_SIZE sizeof(buffer)  // Define the maximum size of the buffer
int buffer_index;                // Index to keep track of the buffer


MODULE_LICENSE("GPL"); // Specifying the license type for the module.
 
 
static struct proc_dir_entry *dir_entry; // a pointer for the proc directory entry.
 
// A function to handle write operations to the proc file
static ssize_t lwl_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos) {
  pr_info("write handler\n"); // Log message for write operation
  return count;               // Return the count, ignoring actual write for now
}
 
// A function to handle read operations from the proc file.
static ssize_t lwl_read(struct file *file, char __user *user_buffer, size_t read_size, loff_t *offset) {
	size_t bytes_read; // Number of bytes actually read
	int maxbytes;      // Number of bytes from offset to MAX_SIZE
	int bytes_to_read; // Number of bytes to read

	pr_info("%s\n", __func__); // Log the function call

	// Calculate the number of bytes to read, ensuring not to exceed the buffer
	maxbytes = MAX_SIZE - *offset;
	if (maxbytes > read_size)
		bytes_to_read = read_size;
	else
		bytes_to_read = maxbytes;

	if (bytes_to_read == 0) {
		pr_info("trying to read past end of device, aborting !!\n");
		return 0; // Return 0 to indicate end of file
	}

	// More details inside the read handler
	// Logging various information for debugging
	pr_info("buffer_index:%d\t offset:%llu\t bytes_to_read:%d\t read_size:%lu\n",
			buffer_index, *offset, bytes_to_read, read_size);
	
	// Copying data to the user's buffer and handling the number of bytes read
	bytes_read = bytes_to_read - copy_to_user(user_buffer, buffer + *offset, bytes_to_read);
	*offset += bytes_read;
	pr_info("\n%s:bytes read=%lu, offset=%d\n", __func__, bytes_read, *(int *)offset);
	return bytes_read; // Return the number of bytes actually read

}

static struct file_operations proc_ops = {
	.owner = THIS_MODULE,
	.read = lwl_read,    // Setting the read handler
	.write = lwl_write,  // Setting the write handler
};

static int proc_init(void) {
	// Create a directory in the proc file system
	dir_entry = proc_mkdir("embedded", NULL);

	// Create the proc file and associate it with file operations
	proc_create("lwl", 0646, dir_entry, &proc_ops);
	pr_info("ldd proc entry created\n"); // Log the creation of the proc file
	return 0; // Return success
}

static void proc_cleanup(void) {
	// Removing the proc file and the directory from the proc filesystem
	remove_proc_entry("lwl", dir_entry);
	remove_proc_entry("embedded", NULL);
}

module_init(proc_init);   // Register the initialization function
module_exit(proc_cleanup); // Register the cleanup function
