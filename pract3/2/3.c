#include<linux/module.h>


static __exit void desd_exit(void)
{
	printk(KERN_INFO "%s : desd_exit() : state = %d\n", THIS_MODULE->name, THIS_MODULE->state);
	printk(KERN_INFO "%s : desd_exit() : deinit is completed successfully\n", THIS_MODULE->name);
}

module_exit(desd_exit);



