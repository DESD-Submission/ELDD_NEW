#include<linux/module.h>
#include<linux/device.h>
#include<linux/fs.h>
#include<linux/cdev.h>

#define MAX 32//buffer size
char buf[MAX];

static dev_t devno;// for allocate device no
static struct class *pclass;// for creating calss

static struct cdev cdev;

//included in fs.h
static int pchar_open(struct inode *pinode, struct file *pfile);
static int pchar_close(struct inode *pinode, struct file *pfile);
static ssize_t pchar_write(struct file *pfile, const char __user *ubuf, size_t size, loff_t *poffset);
static ssize_t pchar_read(struct file *pfile, char __user *ubuf, size_t size, loff_t *poffset);
static loff_t pchar_lseek(struct file *pfile, loff_t offset, int origin);

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = pchar_open,
	.release = pchar_close,
	.write = pchar_write,
	.read = pchar_read,
	.llseek = pchar_lseek
};

static __init int pchar_init(void)
{
    int ret, major,minor;// allocate dev no
    struct device *pdevice;// for creating device

    printk(KERN_INFO "%s : pchar_init() is called\n",THIS_MODULE->name);

    // allocate device number

    ret = alloc_chrdev_region(&devno,0,1,"pchar");
    if(ret < 0)
    {
        printk(KERN_ERR "%s : alloc_chrdev_region() is failed\n",THIS_MODULE->name);
        return ret;
    }

    major = MAJOR(devno);
    minor = MINOR(devno);
    printk(KERN_INFO "%s : alloc_chrdev_region() - allocated devno = %d/%d\n",THIS_MODULE->name,major,minor);
    // allocation complete
    //---------------------------------------------------------------
    // create class for devices
	// USE THIS IN OTHER KERNALS = pclass = class_create(THIS_MODULE,"pchar_class");
    pclass = class_create("pchar_class");
	if(IS_ERR(pclass))
	{
		printk(KERN_ERR "%s : class_create() is failed\n", THIS_MODULE->name);
		
		unregister_chrdev_region(devno, 1);
		return -1;
	}
	printk(KERN_INFO "%s : class_create() - class is created\n", THIS_MODULE->name);
    // class creation is complete
    //---------------------------------------------------------------
    // create device 
	pdevice = device_create(pclass, NULL, devno, NULL, "pchar%d", 0);
	if(IS_ERR(pdevice))
	{
		printk(KERN_ERR "%s : device_create() is failed\n", THIS_MODULE->name);
		class_destroy(pclass);
		unregister_chrdev_region(devno, 1);
		return -1;
	}
	printk(KERN_INFO "%s : device_create() - device is created\n", THIS_MODULE->name);
	//device creation complete 
    /*
    output:- 
    om@om:~/Desktop/Submission/ELDD/PCHAR$ sudo insmod pchar.ko
    [sudo] password for om: 

    om@om:~/Desktop/Submission/ELDD/PCHAR$ sudo dmesg |tail -6
    [   85.449425] audit: type=1400 audit(1718271587.389:182): apparmor="DENIED" operation="capable" class="cap" profile="/snap/snapd/21759/usr/lib/snapd/snap-confine" pid=4003 comm="snap-confine" capability=38  capname="perfmon"
    [  138.474436] pchar: module verification failed: signature and/or required key missing - tainting kernel
    [  138.475079] pchar : pchar_init() is called
    [  138.475082] pchar : alloc_chrdev_region() - allocated devno = 507/0
    [  138.475106] pchar : class_create() - class is created
    [  138.475228] pchar : device_create() - device is created

    om@om:~/Desktop/Submission/ELDD/PCHAR$ ls /dev | grep "pchar"
    pchar0

    om@om:~/Desktop/Submission/ELDD/PCHAR$ ls -l /sys/class/pchar_class/
    total 0
    lrwxrwxrwx 1 root root 0 Jun 13 15:11 pchar0 -> ../../devices/virtual/pchar_class/pchar0
    
    om@om:~/Desktop/Submission/ELDD/PCHAR$ ls /sys/devices/virtual/pchar_class//pchar0
    dev  power  subsystem  uevent


    */
   //----------------------------------------------------------------------
   	// create cdev and add it into cdev map
	cdev_init(&cdev, &f_ops);
	ret = cdev_add(&cdev, devno, 1);
	if(ret < 0)
	{
		printk(KERN_ERR "%s : cdev_add() is failed\n", THIS_MODULE->name);
		
		device_destroy(pclass, devno);
		class_destroy(pclass);
		unregister_chrdev_region(devno, 1);
		return -1;
	}
	printk(KERN_INFO "%s : cdev_add() - device is created\n", THIS_MODULE->name);

    return 0;
}


static __exit void pchar_exit(void)
{
	printk(KERN_INFO "%s : pchar_exit() is called\n", THIS_MODULE->name);
	//------------------------------------------------------------------

	// char device dustroyy
	cdev_del(&cdev);
	printk(KERN_INFO "%s : cdev_del() completed\n", THIS_MODULE->name);
	//------------------------------------------------------------------
	// device distroy
	device_destroy(pclass, devno);
	printk(KERN_INFO "%s : device_destroy() completed\n", THIS_MODULE->name);
	//------------------------------------------------------------------
	// class destroy
	class_destroy(pclass);
	printk(KERN_INFO "%s : class_destroy() completed\n", THIS_MODULE->name);
	//------------------------------------------------------------------
   
	unregister_chrdev_region(devno, 1);
	printk(KERN_INFO "%s : unregister_chrdev_region() completed\n", THIS_MODULE->name);
	//------------------------------------------------------------------
   
	printk(KERN_INFO "%s : pchar_exit() is completed\n", THIS_MODULE->name);
}


//     printk(KERN_INFO "%s : pchar_exit() is called\n",THIS_MODULE->name);
   
//     //------------------------------------------------------------------
   
//     //device destroy
//     device_destroy(pclass, devno);
// 	printk(KERN_INFO "%s : device_destroy() completed\n", THIS_MODULE->name);
//     //----------------------------------------------------------------------
//     // char device destroy
//     unregister_chrdev_region(devno, 1);
// 	printk(KERN_INFO "%s : unregister_chrdev_region() completed\n", THIS_MODULE->name);
//     //----------------------------------------------------------------------
// 	 //class destroyy
//     class_destroy(pclass);
// 	printk(KERN_INFO "%s : class_destroy() completed\n", THIS_MODULE->name);
//     //--------------------------------------------------------------------
// 	// for allocation
//     unregister_chrdev_region(devno, 1);
// 	printk(KERN_INFO "%s : unregister_chrdev_region() completed\n", THIS_MODULE->name);
//     printk(KERN_INFO "%s : pchar_exit() is completed\n", THIS_MODULE->name);
// }


// device operations

static int pchar_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_INFO "%s : pchar_open() is called\n", THIS_MODULE->name);
	return 0;
}

static int pchar_close(struct inode *pinode, struct file *pfile)
{
	printk(KERN_INFO "%s : pchar_close() is called\n", THIS_MODULE->name);
	return 0;
}

static ssize_t pchar_write(struct file *pfile, const char __user *ubuf, size_t size, loff_t *poffset)
{
	int avail, bytes_to_write, nbytes;

	printk(KERN_INFO "%s : pchar_write() is called\n", THIS_MODULE->name);
	
	avail = MAX - *poffset;
	printk(KERN_INFO "%s : pchar_write() : avail bytes = %d\n", THIS_MODULE->name, avail);

	bytes_to_write = avail < size ? avail : size;
	printk(KERN_INFO "%s : pchar_write() : bytes to write = %d\n", THIS_MODULE->name, bytes_to_write);
	
	if(bytes_to_write == 0)
	{
		printk(KERN_ERR "%s : pchar_write() : no space available\n", THIS_MODULE->name);
		return -1;
	}

	nbytes = bytes_to_write - copy_from_user(buf + *poffset, ubuf, bytes_to_write);
	printk(KERN_INFO "%s : pchar_write() : nbytes = %d\n", THIS_MODULE->name, nbytes);
	
	// update file position
	*poffset = *poffset + nbytes;

	return nbytes;
}

static ssize_t pchar_read(struct file *pfile, char __user *ubuf, size_t size, loff_t *poffset)
{
	int avail, bytes_to_read, nbytes;

	printk(KERN_INFO "%s : pchar_read() is called\n", THIS_MODULE->name);
	
	avail = MAX - *poffset;
	printk(KERN_INFO "%s : pchar_read() : avail bytes = %d\n", THIS_MODULE->name, avail);

	bytes_to_read = avail < size ? avail : size;
	printk(KERN_INFO "%s : pchar_read() : bytes to read = %d\n", THIS_MODULE->name, bytes_to_read);
	
	if(bytes_to_read == 0)
	{
		printk(KERN_ERR "%s : pchar_read() : no data available\n", THIS_MODULE->name);
		return -1;
	}

	nbytes = bytes_to_read - copy_to_user(ubuf, buf + *poffset, bytes_to_read);
	printk(KERN_INFO "%s : pchar_read() : nbytes = %d\n", THIS_MODULE->name, nbytes);
	
	// update file position
	*poffset = *poffset + nbytes;

	return nbytes;
}

static loff_t pchar_lseek(struct file *pfile, loff_t offset, int origin)
{
	loff_t newpos = -1;
	printk(KERN_INFO "%s : pchar_lseek() is called\n", THIS_MODULE->name);
	
	switch(origin)
	{
		case SEEK_SET:
			newpos = 0 + offset;
			break;

		case SEEK_CUR:
			newpos = pfile->f_pos + offset;
			break;

		case SEEK_END:
			newpos = MAX + offset;
			break;
	}
	if(newpos < 0)
		newpos = 0;
	if(newpos > MAX)
		newpos = MAX;

	pfile->f_pos = newpos;

	return newpos;
}


module_init(pchar_init);
module_exit(pchar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("om");
MODULE_DESCRIPTION("This is pseudo character device driver");



