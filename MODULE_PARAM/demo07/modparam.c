#include<linux/module.h>

// module parameters
static int arr[4];
module_param_array(arr, int, NULL, 0644);

static __init int modparam_init(void)
{
	printk(KERN_INFO "%s : modparam_init() is called\n", THIS_MODULE->name);
	
	for(int i = 0 ; i < 4 ; i++)
		printk(KERN_INFO "%s : modparam_init() :  arr[%d] = %d\n", THIS_MODULE->name, i, arr[i]);

	printk(KERN_INFO "%s : modparam_init() is finished\n", THIS_MODULE->name);
	return 0;
}

static __exit void modparam_exit(void)
{
	printk(KERN_INFO "%s : modparam_exit() is called\n", THIS_MODULE->name);
	
	for(int i = 0 ; i < 4 ; i++)
		printk(KERN_INFO "%s : modparam_exit() :  arr[%d] = %d\n", THIS_MODULE->name, i, arr[i]);
	
	printk(KERN_INFO "%s : modparam_exit() is finished\n", THIS_MODULE->name);
}

module_init(modparam_init);
module_exit(modparam_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DESD @ SUNBEAM");
MODULE_DESCRIPTION("This is demo of module parameters");










