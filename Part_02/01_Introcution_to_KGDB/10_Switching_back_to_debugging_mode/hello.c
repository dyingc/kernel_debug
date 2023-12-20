#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kgdb.h>

MODULE_LICENSE("GPL");
static int test_hello_init(void)
{
	kgdb_breakpoint();
	return 0;
}

static void test_hello_exit(void)
{
}

module_init(test_hello_init);
module_exit(test_hello_exit);
