#include<linux/module.h>
#include "3.h"

static __exit void desd_exit(void)
{
	my_function();
	printk(KERN_INFO "%s : desd_exit() : state = %d\n", THIS_MODULE->name, THIS_MODULE->state);
	printk(KERN_INFO "%s : desd_exit() : deinit is completed successfully\n", THIS_MODULE->name);
}

module_exit(desd_exit);



