#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define EP_OUT  0x01
#define EP_IN   0x81

static struct usb_device *device;

static int usb_drv_open(struct inode *pinode, struct file *pfile) {
    printk(KERN_INFO "%s: usb_drv_open() called.\n", THIS_MODULE->name);
    return 0;
}

static int usb_drv_close(struct inode *pinode, struct file *pfile) {
    printk(KERN_INFO "%s: usb_drv_close() called.\n", THIS_MODULE->name);
    return 0;
}

static ssize_t usb_drv_read(struct file *pfile, char *ubuf, size_t size, loff_t *poffset) {
    char kbuf[32];
    unsigned int pipe;
    int nbytes, ret;
    printk(KERN_INFO "%s: usb_drv_read() called.\n", THIS_MODULE->name);
    
    pipe = usb_rcvbulkpipe(device, EP_IN);
    memset(kbuf, 0, sizeof(kbuf));
    ret = usb_bulk_msg(device, pipe, kbuf, 32, &nbytes, 5000);
    if(ret != 0)
        printk(KERN_ERR "%s: usb_bulk_msg() failed to receive data.\n", THIS_MODULE->name);
    else
        printk(KERN_INFO "%s: usb_bulk_msg() received data %s.\n", THIS_MODULE->name, kbuf);

    ret = copy_to_user(ubuf, kbuf, nbytes);
    return nbytes;
}

static ssize_t usb_drv_write(struct file *pfile, const char *ubuf, size_t size, loff_t *poffset) {
    char kbuf[4];
    unsigned int pipe;
    int nbytes, ret;
    printk(KERN_INFO "%s: usb_drv_write() called.\n", THIS_MODULE->name);
    
    ret = copy_from_user(kbuf, ubuf, size);

    pipe = usb_sndbulkpipe(device, EP_OUT);
    ret = usb_bulk_msg(device, pipe, kbuf, 2, &nbytes, 5000);
    if(ret != 0)
        printk(KERN_ERR "%s: usb_bulk_msg() failed to send data %s.\n", THIS_MODULE->name, kbuf);
    else
        printk(KERN_INFO "%s: usb_bulk_msg() sent data %s.\n", THIS_MODULE->name, kbuf);
    return size;
}

static struct file_operations usb_ops = {
    .owner = THIS_MODULE,
    .open = usb_drv_open,
    .release = usb_drv_close,
    .write = usb_drv_write,
    .read = usb_drv_read
};

static struct usb_device_id id_table[] = {
    //{ USB_DEVICE(0x0781, 0x558a) },
    { USB_DEVICE(0x0483, 0x5740) },
    { }
};
MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_class_driver class;

static int usb_driver_probe(struct usb_interface *intf, const struct usb_device_id *id) {
    int ret;
    printk(KERN_INFO "%s: probe() called - device %x:%x\n", THIS_MODULE->name, id->idVendor, id->idProduct);

    device = interface_to_usbdev(intf);

    class.fops = &usb_ops;
    class.name = "usb/desd%d";
    ret = usb_register_dev(intf, &class);
    if(ret != 0)
        printk(KERN_ERR "%s : failed to register usb device class.\n", THIS_MODULE->name);
    else
        printk(KERN_ERR "%s : successfully registered usb device class.\n", THIS_MODULE->name);
    return ret;
}

static void usb_driver_disconnect(struct usb_interface *intf) {
    printk(KERN_INFO "%s: disonnect() called\n", THIS_MODULE->name);
    usb_deregister_dev(intf, &class);
    printk(KERN_ERR "%s : successfully deregistered usb device class.\n", THIS_MODULE->name);
}

static struct usb_driver driver = {
    .name = "desd_driver",
    .probe = usb_driver_probe,
    .disconnect = usb_driver_disconnect,
    .id_table = id_table
};

static __init int usb_drv_init(void) {
	int result;
    printk(KERN_INFO "%s : usb_drv_init() called.\n", THIS_MODULE->name);
	result = usb_register(&driver);
    if(result != 0) {
        printk(KERN_ERR "%s : failed to register usb driver.\n", THIS_MODULE->name);
        return result;
    }
    printk(KERN_INFO "%s : usb driver registered.\n", THIS_MODULE->name);
    return 0;
}

static __exit void usb_drv_exit(void) {
    printk(KERN_INFO "%s : usb_drv_exit() called.\n", THIS_MODULE->name);
    usb_deregister(&driver);
    printk(KERN_INFO "%s : usb driver deregistered.\n", THIS_MODULE->name);
}

module_init(usb_drv_init);
module_exit(usb_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nilesh Ghule <nilesh@sunbeaminfo.com>");
MODULE_DESCRIPTION("Simple USB Device Driver");
