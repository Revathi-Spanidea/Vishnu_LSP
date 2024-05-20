/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple Linux device driver (IOCTL)
*
*  \author     saravananb@spanidea.com
*******************************************************************************/
#include <linux/kernel.h> //includes fundamental kernel functions and macros. It provides essential functionalities such as printk for printing messages to the kernel log.
#include <linux/init.h>   //Contains macros for module initialization and cleanup, such as module_init and module_exit
#include <linux/module.h> //This header is necessary for kernel modules. It defines macros like MODULE_LICENSE, MODULE_AUTHOR, and MODULE_DESCRIPTION for module information.
#include <linux/fs.h>     //Includes structures and functions related to the Linux file system, such as file operations (file_operations structure) and the struct file type.
#include <linux/cdev.h>   //Contains definitions and functions related to character devices (cdev structure).
#include<linux/uaccess.h> //copy_to/from_user()Header file for user-space access functions. Includes functions for copying data to/from user space, like copy_to_user and copy_from_user.
#include <linux/ioctl.h>  //IOCTL like IOW, IOR, IO, IOWR are used to communicate with the device drivers. 

 
//a-helps differentiate between different ioctl commands associated with the same driver or device. The command number should be unique for each ioctl operation
//IOW used to create a command code for a write operation. a-used to identify the driver or device associated with the ioctl command.
// int32_t* - passed to the ioctl command. In this case, it's a pointer to a 32-bit integer.
#define WR_VALUE _IOW('a','a',int32_t*) // IOW parameters
#define RD_VALUE _IOR('a','b',int32_t*) //IOR parameters
 
int32_t value = 0;
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

/*
** File operation sturcture
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = etx_read,
        .write          = etx_write,
        .open           = etx_open,
        .unlocked_ioctl = etx_ioctl,
        .release        = etx_release,
};

/*
** This function will be called when we open the Device file
*/
static int etx_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}

/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}

/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Read Function\n");
        return 0;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write function\n");
        return len;
}

/*
** This function will be called when we write IOCTL on the Device file and to add the IOCTL func to our driver.
*/
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{                                                        //cmd -  ioctl command that was called from the userspace.
         switch(cmd) {                                   //arg - source buffer in user space from which the data will be copied.int32_t* -is a typecast, indicating that the data at the user space address 
                case WR_VALUE:                           //&value represents the destination buffer in the kernel where the user space data will be stored.
                        if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )  // Copies data from user space to kernel space. It copies the data pointed to by arg to the kernel variable value.
                        {                                                            //size - specifies the number of bytes to copy from the source buffer to the destination buffer.
                                pr_err("Data Write : Err!\n");
                        }
			printk("DEBUG_PRINT: %d\n",__LINE__);
                        pr_info("Value = %d\n", value);  //The copied value is then printed to the kernel log using pr_info.
			printk("DEBUG_PRINT: %d\n",__LINE__);
                        break;
                case RD_VALUE:
                        if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )  
                        {  //Copies data from kernel space to user space. It copies the value of the kernel variable value to the user space pointed to by arg.
                                pr_err("Data Read : Err!\n");
                        }
                        break;
                default:
                        pr_info("Default\n");
                        break;
        }
        return 0;
}
 
/*
** Module Init function
*/
static int __init etx_driver_init(void)
{
        /*Allocating Major number*/
        if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){    //Dynamically alloacting the major and minor number,Allocates a range of char device numbers (dev_t) dynamically.
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        /*Creating cdev structure*/
        cdev_init(&etx_cdev,&fops);   //Initializes the character device (cdev) structure with the file operations (fops).
 
        /*Adding character device to the system*/
        if((cdev_add(&etx_cdev,dev,1)) < 0){  //Checks if the addition is successful. If not, prints an error message and jumps to the cleanup code (r_class).
            pr_err("Cannot add the device to the system\n");
            goto r_class;
        }
 
        /*Creating struct class*/
        if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class"))){    //owner and name od module.used to create a struct class pointer that can then be used in calls to class_device_create.
            pr_err("Cannot create the struct class\n");                   //The return value can be checked using IS_ERR() macro.
            goto r_class;
        }
 
        //Creates a device file in /dev associated with the class.
        if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"exam_devices"))){  //Class- pointer to the struct class that this device should be registere,
            pr_err("Cannot create the Device 1\n");                       // Parent- pointer to the parent struct device of this new device, devt- dev_t for the char device to be added
            goto r_device;                                                // drvdata-data to be added in dev for the callbacks, fmt-str for the device name
        }                                                                 //checks if the creation is successful. If not, prints an error message and jumps to the cleanup code (r_device).
        pr_info("Device Driver Insert...Done!!!\n");
        return 0;
 
r_device:
        class_destroy(dev_class);   // clean the module.
r_class:
        unregister_chrdev_region(dev,1);   // destroy the class during module cleanup.
        return -1;
}

/*
** Module exit function
*/
static void __exit etx_driver_exit(void)
{
        device_destroy(dev_class,dev);   // destroy the individual device associated with the class.
        class_destroy(dev_class);
        cdev_del(&etx_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saravanan");
MODULE_DESCRIPTION("Simple Linux device driver (IOCTL)");
