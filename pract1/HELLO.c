#include<linux/module.h>

int init_module(void)
{
    printk(KERN_INFO "HELLO : init_module : Heloo DESD @ SUNBEAM\n");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "hello : cleanup_modeule : Bye bye DESD @ SUNBEAM\n");
}


MODULE_INFO(license,"GPL");
MODULE_INFO(author,"Omkar kashid");
MODULE_INFO(description,"This is hello for desd # sunbeam");
