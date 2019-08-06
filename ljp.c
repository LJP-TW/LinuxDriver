#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LJP");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

static int __init ljp_init(void) {
 printk(KERN_INFO "Hello, World!\n");
 return 0;
}

static void __exit ljp_exit(void) {
 printk(KERN_INFO "Goodbye, World!\n");
}

module_init(ljp_init);
module_exit(ljp_exit);
