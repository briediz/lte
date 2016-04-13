
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>

#define TEMPLATE "template"

static char hello_string[]="Hello World\n";

static dev_t template_dev_number;
static struct cdev *driver_object;
struct class *template_class;
static struct device *template_dev;


static int template_open(struct inode *gerate_datei, struct file *instanz)
{
  dev_info(template_dev, "template_open called\n");
  return 0;
}

static int template_read(struct file *instanz, char __user *user, size_t count, loff_t *offset)
{
  dev_info(template_dev, "template_read called\n");
  unsigned long not_copied, to_copy;
  
  to_copy=min(count, strlen(hello_string)+1 );
  not_copied=copy_to_user(user, hello_string, to_copy);
  *offset += to_copy-not_copied;
  return to_copy-not_copied;
}


static struct file_operations fops = {
    .owner= THIS_MODULE,
    .read= template_read,
    .open= template_open,
//    .release=template_close,
};

static int __init template_init(void)
{
  printk( "\t registering %s\n", TEMPLATE);
  if (alloc_chrdev_region(&template_dev_number, 0, 1, TEMPLATE)<0)
    return -EIO;
  driver_object = cdev_alloc();
  if (driver_object==NULL)
    goto free_device_number;
  driver_object->owner = THIS_MODULE;
  driver_object->ops = &fops;
  if(cdev_add(driver_object, template_dev_number, 1))
    goto free_cdev;
  template_class = class_create(THIS_MODULE, TEMPLATE);
  if(IS_ERR(template_class))
    goto free_cdev;
  template_dev=device_create(template_class, NULL, template_dev_number, NULL, "%s", TEMPLATE);
  if(IS_ERR(template_dev)){
    pr_err("hello: device create failed\n");
    goto free_class;
  }
  printk( "\t registering %s complete\n", TEMPLATE);
  
  return 0;
free_class:
  class_destroy(template_class);
free_cdev:
  kobject_put(&driver_object->kobj);
free_device_number:
  unregister_chrdev_region(template_dev_number, 1);
  return -EIO;
}



static void __exit template_exit(void)
{
  device_destroy(template_class, template_dev_number);
  class_destroy(template_class);
  cdev_del(driver_object);
  unregister_chrdev_region(template_dev_number, 1);
  
  printk( "\t unregister complete\n");
  return;
}

module_init(template_init);
module_exit(template_exit);

MODULE_LICENSE("GPL");


