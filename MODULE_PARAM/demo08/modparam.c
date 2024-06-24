#include<linux/module.h>
#include<linux/moduleparam.h>

// module parameters
static int value = 3;

int  notify_value(const char *val, const struct kernel_param *kp)
{
	printk(KERN_INFO "%s : callback to set value is called\n", THIS_MODULE->name);
	int ret = param_set_int(val, kp);
	if(ret == 0)
	{
		printk(KERN_INFO "%s : Value is changed successfully\n", THIS_MODULE->name);
	}
	return 0;
}


struct kernel_param_ops my_ops = {
	.set = notify_value,
	.get = param_get_int
};


module_param_cb(value, &my_ops, &value, 0644);

static __init int modparam_init(void)
{
	printk(KERN_INFO "%s : modparam_init() is called\n", THIS_MODULE->name);
	
	printk(KERN_INFO "%s : modparam_init() : value = %d\n", THIS_MODULE->name, value);

	printk(KERN_INFO "%s : modparam_init() is finished\n", THIS_MODULE->name);
	return 0;
}

static __exit void modparam_exit(void)
{
	printk(KERN_INFO "%s : modparam_exit() is called\n", THIS_MODULE->name);
	
	printk(KERN_INFO "%s : modparam_exit() : value = %d\n", THIS_MODULE->name, value);
	
	printk(KERN_INFO "%s : modparam_exit() is finished\n", THIS_MODULE->name);
}

module_init(modparam_init);
module_exit(modparam_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DESD @ SUNBEAM");
MODULE_DESCRIPTION("This is demo of module parameters");










