#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Sample Kernel Module");

#define DEVICE_NAME "my_ioctl_device"
#define MY_IOCTL_CMD _IOW('k', 1, int)

static struct cdev my_cdev;
static dev_t my_dev;
static int ioctl_value = 0;

static long int my_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    int value;

    switch (cmd) {
        case MY_IOCTL_CMD:
            if (copy_from_user(&value, (int *)arg, sizeof(int))) {
                return -EFAULT;
            }
            printk(KERN_INFO "Received value from user-space: %d\n", value);
            ioctl_value = value;
            break;
        default:
            return -ENOTTY; // Command not recognized
    }
    return 0;
}

static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = my_ioctl,
};

static int __init my_init(void) {
    if (alloc_chrdev_region(&my_dev, 0, 1, DEVICE_NAME) < 0) {
        return -1;
    }

    cdev_init(&my_cdev, &my_fops);

    if (cdev_add(&my_cdev, my_dev, 1) == -1) {
        unregister_chrdev_region(my_dev, 1);
        return -1;
    }

    printk(KERN_INFO "Sample ioctl module initialized.\n");
    return 0;
}

static void __exit my_exit(void) {
    cdev_del(&my_cdev);
    unregister_chrdev_region(my_dev, 1);
    printk(KERN_INFO "Sample ioctl module removed.\n");
}

module_init(my_init);
module_exit(my_exit);

