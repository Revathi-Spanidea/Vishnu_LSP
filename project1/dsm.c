#include<linux/kernek.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/dsm.h>
struct dsm
{
	int data;
}
#define dsm_size (page-size*4)
struct dsm *dptr;
static int __init dsm_init(void)
{
	dptr=(struct dsm*)vmalloc(dsm_size);
	if(dptr==NULL)
	{
		printk(KERN_INFO "Unable to allocate memory..\n");
		exit(0);
	}
	dptr->data=0;
	printk(KERN_INFO "MEMORY is allocated..\n");
	return 0;
}

static void __exit dsm_init(void)
{
	vfree(dptr);
	printk(KERN_INFO "memroy is deallocated..\n");
}

module_init(dsm_init);
module_exit(dsm_exit);

MODULE_VERSION("1.2");
MODULE_DESCRIPTION("DSM MODEULE");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("VISHNU");

