#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_DESCRIPTION("42cdev driver");
MODULE_AUTHOR("Rémi LAFORGUE");
MODULE_LICENSE("GPL");

static dev_t dev_nr;
static struct class *fortytwo_class;
static struct cdev fortytwo_cdev;

static ssize_t my_read(struct file *f, char __user *u, size_t l, loff_t *o)
{
	printk(KERN_INFO "42cdev - relaforg\n");
	return (0);
}

static struct file_operations fops = {
	.read = my_read
};

static int __init custom_init(void)
{
	int status;

	status = alloc_chrdev_region(&dev_nr, 0, MINORMASK + 1, "fortytwo");
	if (status)
	{
		pr_err("42cdev - Error reserving the region of device numbers\n");
		return (status);
	}

	cdev_init(&fortytwo_cdev, &fops);
	fortytwo_cdev.owner = THIS_MODULE;

	status = cdev_add(&fortytwo_cdev, dev_nr, MINORMASK + 1);
	if (status)
	{
		pr_err("42cdev - Error adding cdev\n");
		goto free_devnr;
	}

	printk(KERN_INFO  "42cdev - Registered a character device for Major %d starting with Minor %d\n", MAJOR(dev_nr), MINOR(dev_nr));

	fortytwo_class = class_create("fortytwo");
	if (!fortytwo_class)
	{
		pr_err("42cdev - Could not create class fortytwo\n");
		status = ENOMEM;
		goto delete_cdev;
	}

	if (!device_create(fortytwo_class, NULL, dev_nr, NULL, "fortytwo"))
	{
		pr_err("42cdev - Could not create device fortytwo\n");
		status = ENOMEM;
		goto delete_class;
	}

	pr_info("42cdev - Created device under /sys/class/fortytwo/fortytwo");
        return (0);

delete_class:
	class_unregister(fortytwo_class);
	class_destroy(fortytwo_class);
delete_cdev:
	cdev_del(&fortytwo_cdev);
free_devnr:
	unregister_chrdev_region(dev_nr, MINORMASK + 1);
	return (status);
}

static void __exit custom_exit(void)
{
	device_destroy(fortytwo_class, dev_nr);
	class_unregister(fortytwo_class);
	class_destroy(fortytwo_class);
	cdev_del(&fortytwo_cdev);
	unregister_chrdev_region(dev_nr, MINORMASK + 1);
}

module_init(custom_init);
module_exit(custom_exit);
