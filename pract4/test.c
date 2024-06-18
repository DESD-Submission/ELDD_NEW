#include<linux/module.h>
#include<linux/init.h>
#include<linux/device.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/kfifo.h>
#include<linux/uaccess.h>

static int pchar_open(struct inode *,struct file *);
static int pchar_close(struct inode *, struct file *);
static ssize_t pchar_read(struct file *,char *,size_t,loff_t *);
static ssize_t pchar_write(struct file *,const char *, size_t,loff_t *);

#define MAX 32
static struct kfifo buf;
static int major;
static dev_t devno;
static struct class *pcclass;
static struct cdev cedv;
static struct file_operations pchar_fops = 
{
    .owner = THIS_MODULE,
    .open = pchar_open,
    .release = pchar_close,
    .read = pchar_read,
    .write = pchar_write
};

static __init int pchar_init(void)
{
    int ret, minor;
    struct device *pdevice;

    printk( KERN_INFO "%s : pchar_init() called.\n", THIS_MODULE->name);
    ret = kfifo_alloc(&buf,MAX,GFP_KERNEL);
    if(ret != 0)
    {
        printk(KERN_ERR "%s : kfifo_alloc() failed.\n",THIS_MODULE->name);
        goto kfifo_alloc_faoled;
    }
    printk(KERN_ERR "%s : kfifo_alloc successfully created device.\n",THIS_MODULE->name);

    ret = allco_chrdev_region(&devno,0,1,"pchar");
    if(ret != 0)
    {
        printk(KERN_ERR "%s : alloc_chadev_region() failed.\n",THIS_MODULE->name);
        goto alloc_chrdev_region_failed;
    }
    major = MAJOR(devno);
    minor = MINOR(devno);
    printk(KERN_INFO "%s : alloc_chredv_region() failed ");

}


