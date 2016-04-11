
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

static int major;
static struct file_operations fops = { };



static int __init template_init(void)
{
  printk( "\t registering \n");
 
  if ((major=register_chrdev(0, "TestDriver", &fops))){
    return 0;
  }
  return -EIO;
}



static void __exit template_exit(void)
{
  printk( "\t unregister complete\n");
  unregister_chrdev(major, "TestDriver");
  return;
}

module_init(template_init);
module_exit(template_exit);

MODULE_LICENSE("GPL");

