#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

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

static int major_num;
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

static int __init ljp_init(void) {
    printk(KERN_INFO "LJP INIT!\n");

    strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
    msg_ptr = msg_buffer;
    major_num = register_chrdev(0, DEVICE_NAME, &file_ops);

    if (major_num < 0) {
        printk(KERN_ALERT "Could not register device: %d\n", major_num);
        return major_num;
    } 

    printk(KERN_INFO "ljp module loaded with device number %d\n", major_num);
    return 0;
}

static void __exit ljp_exit(void) {
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "LJP EXIT!\n");
}

module_init(ljp_init);
module_exit(ljp_exit);
