#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <asm/uaccess.h>


static dev_t gpio_dev_number;
static struct cdev *driver_object;
static struct class *gpio_class;
static struct device *gpio_dev;
static int rpi_irq_17;
static char *devname = "gpio_irq";
static wait_queue_head_t sleeping_for_ir;
static int interrupt_arrived;




static irqreturn_t rpi_gpio_isr( int irq, void *data)
{
  printk("rpio_gpio_isr( %d, %p )\n", irq, data);
  interrupt_arrived += 1;
  wake_up(&sleeping_for_ir);
  return IRQ_HANDLED;
}

static int config_gpio( int gpionr )
{
  int err, rpi_irq;
  char name[20];
  snprintf( name, sizeof(name), "rpi-gpio-%d", gpionr);
  err = gpio_request(gpionr, name);
  if(err){
    printk("gpio_request failed %d\n", err);
    return -1;
  }
  
  err = gpio_direction_inpu( gpionr );
  if(err){
    printk("gpio_request failed %d\n", err);
    gpio_free( gpionr );
    return -1;
  }
  
  rpi_irq = gpio_to_irq( gpionr );
  printk("gpio_to_irq returned %d\n", rpi_irq);
  if(rpi_irq < 0){
    printk("gpio_request failed %d\n", rpi_irq);
    gpio_free( gpionr );
    return -1;
  }
  
  err = request_irq( rpi_irq, rpi_gpio_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, 
		     devname, driver_object );
  printk("driver_object: %p\n", driver_object);
  if(err){
    printk("request_irq failed with %d\n", err);
    gpio_free( gpionr );
    return -1;
  }
  printk("gpio % succesfully configured: %p\n", gpionr);
  return rpi_irq;
}

static driver_open( struct inode *geraete_datei, struct file *instanz )
{
  printk("driver_open called\n");
  return 0;
}

static driver_close( struct inode *geraete_datei, struct file *instanz )
{
  printk("driver_close called\n");
  return 0;
}

static ssize_t driver_read(struct file *instanz, char __user *user, 
			   size_t count, loff_t *offset)
{
  unsigned long not_copied, to_copy;
  
  interrupt_arrived = 0;
  wait_event_interruptible( sleeping_for_ir, interrupt_arrived);
  to_copy = min( count, sizeof(interrupt_arrived));
  not_copied = copy_to_user(user, &interrupt_arrived, to_copy);
  return to_copy-not_copied;
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .read = driver_read,
  .open = driver_open,
  .release = driver_close,
};

static int __init mod_init (void)
{
  dev_info(
  
}
 
static int __init mod_exit (void)
{
  dev_info(gpio_dev, "mod_exit");
  device_destroy(gpio_class, gpio_dev_number);
  class_destroy( gpio_class );
  cdev_del( driver_object );
  unregister_chrdev_region( gpio_dev_number, 1);
  free_irq( rpi_irq_17, driver_object );
  gpio_free( 17 );
}

module_init(mod_init);
module_exit(mod_exit);

