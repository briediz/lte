
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define TEMPLATE "template"


static dev_t template_dev_number;
static struct cdev *driver_object;
struct class *template_class;


static struct file_operations fops = {
  
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
  device_create(template_class, NULL, template_dev_number, NULL, "%s", TEMPLATE);
  printk( "\t registering %s complete\n", TEMPLATE);
  
  return 0;
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


