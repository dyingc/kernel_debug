#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
static int test_warn_init(void)
{
    printk(KERN_INFO"%s: In init\n", __func__);
    WARN(1, "LWL");
    return 0;
}

static void test_warn_exit(void)
{
    printk(KERN_INFO"%s: In exit\n", __func__);
}

module_init(test_warn_init);
module_exit(test_warn_exit);

