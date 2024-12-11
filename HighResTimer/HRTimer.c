#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>

static struct hrtimer timer;
u64 start_time;

#define TIMEOUT 

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("An Example of linux kernel module High Resolution timer");

/**
* @brief This function is called when the module is loaded into the kernel.
*/

static enum hrtimer_restart hrtimer_handler(struct hrtimer *the_timer) {
	/* Get current time */
	u64 now_time = jiffies;
	printk("start time - Current time = %u\n", jiffies_to_msecs(now_time - start_time));
	return HRTIMER_NORESTART;
}

/**
* @brief This function is called when the module is loaded into the kernel.
*/

static int __init ModuleInit(void)
{
  printk("Hello World ! \n");
  //Setup the timer and its callback.
  hrtimer_init(&timer,CLOCK_MONOTONIC,HRTIMER_MODE_REL);

  //set the timer timeout handler.
  timer.function = &hrtimer_handler;
	start_time = jiffies;

  //Modifies the timer timeout and starts the timer.
  hrtimer_start(&timer,ms_to_ktime(100), HRTIMER_MODE_REL);
  return 0; //Indicating success.
}

/**
*  @brief This function is called when the module is removed from memory.
*/

static void __exit ModuleExit(void)
{
  hrtimer_cancel(&timer);
  printk("Goodbye Kernel !\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);