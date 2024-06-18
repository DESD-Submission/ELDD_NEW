#include<linux/module.h>

static __init int desd_init(void)
{
    printk(KERN_INFO "%s : desd_init() : state = %d\n", THIS_MODULE->name,THIS_MODULE->state);
    printk(KERN_INFO "%s : desd_init() : init is successfully completed\n",THIS_MODULE->name);
    return 0;
}

module_init(desd_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OMKAR KASHID");
MODULE_DESCRIPTION("THIS IS SPLIT MODULE");
