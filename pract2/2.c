#include<linux/module.h>

static __init int desd_init(void)
{
    printk(KERN_INFO "%s : desd_init : init is successful\n",THIS_MODULE->name);
    return 0;
}

static __exit void desd_exit(void)
{
    printk(KERN_INFO "%s : desd_init : exit is successful\n",THIS_MODULE->name);

}

module_init(desd_init);
module_exit(desd_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("OMAKR KASHID");
MODULE_DESCRIPTION("This is eldd - desd module");
