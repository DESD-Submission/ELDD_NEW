#include<linux/module.h>

int my_var = 10;

void my_function(void)
{
	printk(KERN_INFO "%s : my_function() is called\n", THIS_MODULE->name);
	printk(KERN_INFO "%s : my_function() : my_var = %d\n", THIS_MODULE->name, my_var);
}






