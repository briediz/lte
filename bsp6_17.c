#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/module.h>	
#include <linux/kernel.h>	
#include <linux/init.h>		
#include <linux/delay.h>		
#define DRIVER_AUTHOR "mail@info.eu"
#define DRIVER_DESC   "A sample driver"

static struct workqueue_struct *wq;



static void work_queue_func( struct work_struct *work)
{
  int i;
//   printk("-->  work_queue_function ( %p, jiffies: %ld ) %d\n", data, jiffies, current->pid);
  for (i=0; i<60; i++){
    printk("printk: work_queue_func...%d \n", i);
    pr_info("pr_info: work_queue_func...%d \n\n", i);
    msleep(1000);
  }
  return;
}

static void work_queue_func2( struct work_struct *work)
{
  int i;
//   printk("-->  work_queue_function2 ( %p, jiffies: %ld ) %d\n", data, jiffies, current->pid);
  for (i=0; i<60; i++){
    printk("printk: work_queue_func2...%d \n", i);
    pr_info("pr_info: work_queue_func2...%d \n\n", i);
    msleep(1000);
  }
  return;
}

static DECLARE_WORK(work_obj, work_queue_func);

static DECLARE_WORK(work_obj2, work_queue_func2);

static int __init mod_init (void)
{
  wq = create_workqueue("my_wq");
  
  if( queue_work (wq, &work_obj)) {
    printk("queue work succesfully... \n");
  } else {
    printk("queue work not succesfully... \n");
  }
  
  
  if( queue_work (wq, &work_obj)) {
    printk("queue work 1 succesfully... \n");
  } else {
    printk("queue work 1 not succesfully... \n");
  }
  
  
  if( queue_work (wq, &work_obj2)) {
    printk("queue work 2 succesfully... \n");
  } else {
    printk("queue work 2 not succesfully... \n");
  }
  return 0;
}
 
static void __init mod_exit (void)
{
  printk("module exit called... \n");
  if( wq ) {
    destroy_workqueue(wq);
    printk("workqueue destroy... \n");
  }
      
}

module_init(mod_init);
module_exit(mod_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);	
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");