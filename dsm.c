#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/version.h>

// Define DSM structure
struct dsm_data {
    int data;
};

// Define DSM size
#define DSM_SIZE (PAGE_SIZE * 4)  // 4 pages

// Global variable for DSM
struct dsm_data *dsm_ptr;

// DSM initialization function
static int __init dsm_init(void)
{
    // Allocate memory for DSM
    dsm_ptr = (struct dsm_data *)vmalloc(DSM_SIZE);
    if (!dsm_ptr)
    {
        printk(KERN_ERR "Failed to allocate DSM memory\n");
        return -ENOMEM;
    }

    // Initialize DSM data
    dsm_ptr->data = 0;

    printk(KERN_INFO "DSM module initialized\n");
    return 0;
}

// DSM cleanup function
static void __exit dsm_exit(void)
{
    // Free DSM memory
    vfree(dsm_ptr);

    printk(KERN_INFO "DSM module removed\n");
}

// Module initialization and exit macros
module_init(dsm_init);
module_exit(dsm_exit);

// Module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Saravanan");
MODULE_DESCRIPTION("Distributed Shared Memory (DSM) Module");

