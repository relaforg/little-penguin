#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

MODULE_DESCRIPTION("HelloWorld kernel module");
MODULE_AUTHOR("Rémi LAFORGUE");
MODULE_LICENSE("GPL");

static int __init custom_init(void)
{
        printk(KERN_INFO "Hello world!\n");
        return (0);
}

static void __exit custom_exit(void)
{
        printk(KERN_INFO "Cleaning up module.\n");
}

module_init(custom_init);
module_exit(custom_exit);
