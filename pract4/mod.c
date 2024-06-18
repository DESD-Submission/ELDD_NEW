#include<linux/module.h>

// module parameters
static int value = 3;
module_param(value, int, 0644);

static char *name = "desd";
module_param_named(u_name, name, charp, 0644);

static __init int modparam_init(void)
{
	printk(KERN_INFO "%s : modparam_init() is called\n", THIS_MODULE->name);
	
	printk(KERN_INFO "%s : modparam_init() : value = %d\n", THIS_MODULE->name, value);
	for(int i = 0 ; i < value ; i++)
		printk(KERN_INFO "%s : modparam_init() : name = %s\n", THIS_MODULE->name, name);

	printk(KERN_INFO "%s : modparam_init() is finished\n", THIS_MODULE->name);
	return 0;
}

static __exit void modparam_exit(void)
{
	printk(KERN_INFO "%s : modparam_exit() is called\n", THIS_MODULE->name);
	
	printk(KERN_INFO "%s : modparam_exit() : value = %d\n", THIS_MODULE->name, value);
	for(int i = 0 ; i < value ; i++)
		printk(KERN_INFO "%s : modparam_exit() : name = %s\n", THIS_MODULE->name, name);
	
	printk(KERN_INFO "%s : modparam_exit() is finished\n", THIS_MODULE->name);
}

module_init(modparam_init);
module_exit(modparam_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DESD @ SUNBEAM");
MODULE_DESCRIPTION("This is demo of module parameters");









