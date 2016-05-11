#include <linux/fs.h>


static dev_t gpio_dev_number;

static irqreturn_t rpi_gpio_isr( int irq, void *data)
{
}

static int config_gpio( int gpionr )
{
  err = gpio_request(gpionr, name);
  
  err = gpio_direction_inpu( gpionr );
  
  rpi_irq = gpio_to_irq( gpionr );
  
  err = request_irq( rpi_irq, rpi_gpio_isr, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, 
		     devname, driver_object );
  
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
  
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .read = driver_read,
  .open = driver_open,
  .release = driver_close,
};

static int __init mod_init (void)
{
  
}
 
static int __init mod_exit (void)
{
  
}

module_init(mod_init);
module_exit(mod_exit);

