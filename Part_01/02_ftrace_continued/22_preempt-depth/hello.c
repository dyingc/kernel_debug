#include <linux/kernel.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");


static int test_hello_init(void)
{

	trace_printk("%s: In init\n", __func__);
	preempt_disable();
	trace_printk("Disabled preemption:%d\n", smp_processor_id());
	preempt_enable();
	trace_printk("Enabled preemption:%d\n", smp_processor_id());
	return 0;

}
static void test_hello_exit(void)
{
	trace_printk("%s: In exit\n", __func__);
}

module_init(test_hello_init);
module_exit(test_hello_exit);
		
