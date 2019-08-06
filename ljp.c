#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LJP");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");

#define DEVICE_NAME "ljp"
#define EXAMPLE_MSG "Hello LJP!\n"
#define MSG_BUFFER_LEN 12

/* Prototypes for device functions */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static int major_num = -1;
static struct cdev mycdev;
static struct class *myclass = NULL;

static int device_open_count = 0;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;

static struct file_operations file_ops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static ssize_t device_read(struct file *fp, char *buffer, size_t len, loff_t *offset) {
    int bytes_read = 0;
    while (len)
    {
        if (*msg_ptr == 0) {
            msg_ptr = msg_buffer;
        }

        put_user(*(msg_ptr++), buffer++);
        len--;
        bytes_read++;
    }
    return bytes_read;
}

static ssize_t device_write(struct file *fp, const char *buffer, size_t len, loff_t *offset) {
    printk(KERN_ALERT "This operation is not supported.\n");
    return -EINVAL;
}

static int device_open(struct inode *inode, struct file *fp) {
    if (device_open_count) {
        return -EBUSY;
    }
    device_open_count++;
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *inode, struct file *fp) {
    device_open_count--;
    module_put(THIS_MODULE);
    return 0;
}

static int my_dev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static cleanup(int device_created)
{
    if (device_created) {
        device_destroy(myclass, major_num);
        cdev_del(&mycdev);
    }

    if (myclass)
        class_destroy(myclass);

    if (major_num != -1)
        unregister_chrdev_region(major_num, 1);
}

static int __init ljp_init(void) {
    printk(KERN_INFO "LJP INIT!\n");

    int device_created = 0;

    strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
    msg_ptr = msg_buffer;

    if (alloc_chrdev_region(&major_num, 0, 1, DEVICE_NAME "_proc") < 0)
        goto error;
    if ((myclass = class_create(THIS_MODULE, DEVICE_NAME "_sys")) == NULL)
        goto error;

    myclass->dev_uevent = my_dev_uevent;
    
    if (device_create(myclass, NULL, major_num, NULL, DEVICE_NAME "_dev") == NULL)
        goto error;

    device_created = 1;
    cdev_init(&mycdev, &file_ops);
    if (cdev_add(&mycdev, major_num, 1) == -1)
        goto error;

    return 0;

error:
    cleanup(device_created);
    return -1;
}

static void __exit ljp_exit(void) {
    cleanup(1);
    printk(KERN_INFO "LJP EXIT!\n");
}

module_init(ljp_init);
module_exit(ljp_exit);
