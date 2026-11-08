/**
 * @file kernelwise_char.c
 * @brief Production Linux Loadable Kernel Module (LKM) Character Device Driver.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright GNU General Public License v2 (GPL-2.0)
 *
 * Demonstrates:
 *  - Dynamic major/minor allocation (alloc_chrdev_region)
 *  - Character device registration (cdev_init, cdev_add)
 *  - Automatic device node creation (/dev/kernelwise_char via class_create & device_create)
 *  - Virtual File System (VFS) file_operations (open, release, read, write, unlocked_ioctl)
 *  - Kernel space thread synchronization (mutex)
 *  - Fault-tolerant user-to-kernel memory copies (copy_to_user, copy_from_user)
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

#include "char_driver_ioctl.h"

#define DRIVER_NAME "kernelwise_char"
#define CLASS_NAME  "kernelwise_class"
#define BUFFER_SIZE 2048

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aravind Reddy <Kernelwise Labs>");
MODULE_DESCRIPTION("Enterprise Linux Character Device Driver with IOCTL and Mutex Synchronization");
MODULE_VERSION("1.0.0");

static dev_t dev_num;
static struct cdev kw_cdev;
static struct class *kw_class = NULL;
static struct device *kw_device = NULL;

static DEFINE_MUTEX(kw_mutex);
static char kernel_buffer[BUFFER_SIZE];
static size_t data_length = 0;
static kw_driver_stats_t driver_stats = {
    .buffer_capacity = BUFFER_SIZE,
    .bytes_stored = 0,
    .open_count = 0,
    .total_reads = 0,
    .total_writes = 0
};

static int dev_open(struct inode *inodep, struct file *filep) {
    if (!mutex_trylock(&kw_mutex)) {
        pr_warn("%s: Device is currently busy (held by another process)\n", DRIVER_NAME);
        return -EBUSY;
    }
    driver_stats.open_count++;
    pr_info("%s: Device opened (Active sessions: %u)\n", DRIVER_NAME, driver_stats.open_count);
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    mutex_unlock(&kw_mutex);
    pr_info("%s: Device closed gracefully\n", DRIVER_NAME);
    return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset) {
    size_t bytes_to_read;
    size_t uncopied;

    if (*offset >= data_length) {
        return 0; // EOF
    }

    bytes_to_read = len;
    if (bytes_to_read > (data_length - *offset)) {
        bytes_to_read = data_length - *offset;
    }

    uncopied = copy_to_user(buffer, kernel_buffer + *offset, bytes_to_read);
    if (uncopied) {
        pr_err("%s: Failed to copy %zu bytes to user space\n", DRIVER_NAME, uncopied);
        return -EFAULT;
    }

    *offset += bytes_to_read;
    driver_stats.total_reads++;
    pr_info("%s: Read %zu bytes (Current offset: %lld)\n", DRIVER_NAME, bytes_to_read, *offset);
    return bytes_to_read;
}

static ssize_t dev_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) {
    size_t bytes_to_write;
    size_t uncopied;

    if (*offset >= BUFFER_SIZE) {
        return -ENOSPC; // Buffer full
    }

    bytes_to_write = len;
    if (bytes_to_write > (BUFFER_SIZE - *offset)) {
        bytes_to_write = BUFFER_SIZE - *offset;
    }

    uncopied = copy_from_user(kernel_buffer + *offset, buffer, bytes_to_write);
    if (uncopied) {
        pr_err("%s: Failed to copy %zu bytes from user space\n", DRIVER_NAME, uncopied);
        return -EFAULT;
    }

    *offset += bytes_to_write;
    if (*offset > data_length) {
        data_length = *offset;
    }
    driver_stats.bytes_stored = (uint32_t)data_length;
    driver_stats.total_writes++;
    pr_info("%s: Wrote %zu bytes (Total buffered: %zu)\n", DRIVER_NAME, bytes_to_write, data_length);
    return bytes_to_write;
}

static long dev_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
        case KW_IOCTL_RESET_BUFFER:
            pr_info("%s: IOCTL Reset buffer received\n", DRIVER_NAME);
            memset(kernel_buffer, 0, BUFFER_SIZE);
            data_length = 0;
            driver_stats.bytes_stored = 0;
            break;

        case KW_IOCTL_GET_STATS:
            if (copy_to_user((void __user *)arg, &driver_stats, sizeof(kw_driver_stats_t))) {
                pr_err("%s: IOCTL GET_STATS copy_to_user failed\n", DRIVER_NAME);
                return -EFAULT;
            }
            break;

        case KW_IOCTL_SET_MAX_MSG:
            pr_info("%s: IOCTL SET_MAX_MSG received (value: %lu)\n", DRIVER_NAME, arg);
            break;

        default:
            pr_warn("%s: Unknown IOCTL command: 0x%X\n", DRIVER_NAME, cmd);
            return -EINVAL;
    }
    return 0;
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .open           = dev_open,
    .release        = dev_release,
    .read           = dev_read,
    .write          = dev_write,
    .unlocked_ioctl = dev_ioctl,
};

static int __init kw_driver_init(void) {
    int ret;
    pr_info("%s: Initializing driver module\n", DRIVER_NAME);

    // 1. Dynamic allocation of major and minor numbers
    ret = alloc_chrdev_region(&dev_num, 0, 1, DRIVER_NAME);
    if (ret < 0) {
        pr_err("%s: Failed to allocate major number\n", DRIVER_NAME);
        return ret;
    }
    pr_info("%s: Allocated Major = %d, Minor = %d\n", DRIVER_NAME, MAJOR(dev_num), MINOR(dev_num));

    // 2. Initialize and bind cdev
    cdev_init(&kw_cdev, &fops);
    kw_cdev.owner = THIS_MODULE;
    ret = cdev_add(&kw_cdev, dev_num, 1);
    if (ret < 0) {
        pr_err("%s: Failed to register cdev\n", DRIVER_NAME);
        unregister_chrdev_region(dev_num, 1);
        return ret;
    }

    // 3. Create sysfs class
    kw_class = class_create(CLASS_NAME);
    if (IS_ERR(kw_class)) {
        pr_err("%s: Failed to create sysfs class\n", DRIVER_NAME);
        cdev_del(&kw_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(kw_class);
    }

    // 4. Create device node under /dev
    kw_device = device_create(kw_class, NULL, dev_num, NULL, DRIVER_NAME);
    if (IS_ERR(kw_device)) {
        pr_err("%s: Failed to create device node /dev/%s\n", DRIVER_NAME, DRIVER_NAME);
        class_destroy(kw_class);
        cdev_del(&kw_cdev);
        unregister_chrdev_region(dev_num, 1);
        return PTR_ERR(kw_device);
    }

    pr_info("%s: Driver registered successfully at /dev/%s\n", DRIVER_NAME, DRIVER_NAME);
    return 0;
}

static void __exit kw_driver_exit(void) {
    pr_info("%s: Unloading driver module\n", DRIVER_NAME);
    device_destroy(kw_class, dev_num);
    class_destroy(kw_class);
    cdev_del(&kw_cdev);
    unregister_chrdev_region(dev_num, 1);
    pr_info("%s: Driver unregistered and resources freed\n", DRIVER_NAME);
}

module_init(kw_driver_init);
module_exit(kw_driver_exit);
