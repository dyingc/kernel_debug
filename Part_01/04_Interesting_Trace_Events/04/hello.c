#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
MODULE_LICENSE("GPL");


static int test_hello_init(void)
{

	trace_printk("%s: In init before schedule\n", __func__);
	set_current_state(TASK_INTERRUPTIBLE);
	schedule();
	trace_printk("%s: In init after schedule\n", __func__);
	return 0;

}
static void test_hello_exit(void)
{
	trace_printk("%s: In exit\n", __func__);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
		
