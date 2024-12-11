#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

static struct timer_list timer;

#define TIMEOUT 

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("An Example of linux kernel module Timer");

/**
* @brief This function is called when the module is loaded into the kernel.
*/

void timer_callback(struct timer_list *data)
{
  printk("Timer Expired ! \n");
}

/**
* @brief This function is called when the module is loaded into the kernel.
*/

static int __init ModuleInit(void)
{
  printk("Hello World ! \n");
  //Setup the timer and its callback.
  timer_setup(&timer,timer_callback,0);
  //Modifies the timer timeout and starts the timer.

  //Note: Jiffies is the timeperiod between each clock tick.
  mod_timer(&timer,jiffies + msecs_to_jiffies(5000));
  return 0; //Indicating success.
}

/**
*  @brief This function is called when the module is removed from memory.
*/

static void __exit ModuleExit(void)
{
  del_timer(&timer);
  printk("Goodbye Kernel !\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
