#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/sched.h>


MODULE_LICENSE("GPL");

char tasklet_data[] = "linux kernel is very easy";

void print_context(void)
{
        if (in_softirq()) {
                trace_printk("Code is running in soft irq context\n");
        } else {
                trace_printk("Code is not running in soft irq context\n");
        }

}


void tasklet_function(unsigned long data)
{
	trace_printk("%s:data:%s\n", __func__, (char *)data);
	print_context();
	return;
}


static struct tasklet_struct *my_tasklet;

static int test_tasklet_init(void)
{
        trace_printk("%s: In init\n", __func__);
	my_tasklet = kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);
	tasklet_init(my_tasklet, tasklet_function, tasklet_data);
	tasklet_schedule(my_tasklet);	
	return 0;
}

static void test_tasklet_exit(void)
{
        trace_printk("%s: In exit\n", __func__);
	kfree(my_tasklet);
}

module_init(test_tasklet_init);
module_exit(test_tasklet_exit);
