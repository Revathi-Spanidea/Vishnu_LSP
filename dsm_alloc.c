#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "dsm_module"
#define MAX_MEM_SIZE (1024 * 1024) // Maximum shared memory size in bytes

static int major_num;
static void *shared_memory;

// Function prototypes
static int dsm_open(struct inode *, struct file *);
static int dsm_release(struct inode *, struct file *);
static ssize_t dsm_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t dsm_write(struct file *, const char __user *, size_t, loff_t *);

// File operations structure
static struct file_operations fops = {
    .open = dsm_open,
    .release = dsm_release,
    .read = dsm_read,
    .write = dsm_write,
};

// Module initialization
static int __init dsm_module_init(void)
{
    // Register character device
    major_num = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_num < 0)
    {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_num;
    }
    
    // Allocate shared memory
    shared_memory = kmalloc(MAX_MEM_SIZE, GFP_KERNEL);
    if (!shared_memory)
    {
        unregister_chrdev(major_num, DEVICE_NAME);
        printk(KERN_ALERT "Failed to allocate shared memory\n");
        return -ENOMEM;
    }

    printk(KERN_INFO "DSM module registered with major number %d\n", major_num);
    return 0;
}

// Module cleanup
static void __exit dsm_module_exit(void) 
{
    // Free shared memory
    kfree(shared_memory);
    
    // Unregister character device
    unregister_chrdev(major_num, DEVICE_NAME);
    printk(KERN_INFO "Unregistered DSM module\n");
}

// Called when the device is opened
static int dsm_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "DSM device opened\n");
    return 0;
}

// Called when the device is released
static int dsm_release(struct inode *inodep, struct file *filep) 
{
    printk(KERN_INFO "DSM device closed\n");
    return 0;
}

// Called when data is read from the device
static ssize_t dsm_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset)
{
    // Copy data from shared memory to user buffer
    if (copy_to_user(buffer, shared_memory + *offset, len) != 0)
        return -EFAULT; // Error in copying data
    
    *offset += len;
    return len;
}

// Called when data is written to the device
static ssize_t dsm_write(struct file *filep, const char __user *buffer, size_t len, loff_t *offset) 
{
    // Copy data from user buffer to shared memory
    if (copy_from_user(shared_memory + *offset, buffer, len) != 0)
        return -EFAULT; // Error in copying data
    
    *offset += len;
    return len;
}

module_init(dsm_module_init);
module_exit(dsm_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saravanan");
MODULE_DESCRIPTION("Distributed Shared Memory Module");

