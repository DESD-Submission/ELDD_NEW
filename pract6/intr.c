#include<linux/module.h>
#include<linux/init.h>
#include<linux/device.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/gpio.h>
#include<linux/uaccess.h>
#include<linux/interrupt.h>

static dev_t devno;//alloc_chrdev_region //1
static int major;//alloc_chrdev_region //1
static struct class *pclass;//class_create //2

static __init int bbb_gpio_init (void)
{
    int ret, minor;

    printk(KERN_INFO "%s : bbb_gpio_init() called.\n", THIS_MODULE->name);
    //---------------------------//1
    ret = alloc_chrdev_region(&devno,0,1,"bbb_gpio");
    if(ret != 0){
        goto alloc_chrdev_region_failed;
    }
    major = MAJOR(devno);
    minor = MINOR(devno);
    printk(KERN_INFO "%s: alloc_chrdev_region() allocated device number %d/%d.\n", THIS_MODULE->name, major, minor);

    //===============================

    //-----------------------------//2
    pclass = class_create( "bbb_gpio_class");
    if(IS_ERR(pclass)){
        ret = -1;
        goto class_create_failed;
    }



    return 0;
    class_create_failed://2
    unregister_chrdev_region(devno,1);
    alloc_chrdev_region_failed://1
    return ret;
}

static __exit void bbb_gpio_exit(void)
{
    printk(KERN_INFO"%s bbb_gpio_exit() called.\n",THIS_MODULE->name);

    //======================================//1
    unregister_chrdev_region(devno,1);
    //---------------------------------------//2
    class_destroy(pclass);

}

module_init(bbb_gpio_init);
module_exit(bbb_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OMAKR KASHID <omkarkashidok@gmail.com>");
MODULE_DESCRIPTION("");




