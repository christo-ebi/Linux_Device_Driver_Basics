#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/delay.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("A Linux Kernel Module - Kernel Threads example");


struct task_struct *thread1_ptr = NULL;
struct task_struct *thread2_ptr = NULL;

int thread_data1 = 1;
int thread_data2 = 2;

int Thread1(void *data)
{
	unsigned int counter = 0;
	int thread_no = *(int *) data;

	/* Loop to run until the kthread_stop is called */
	while(!kthread_should_stop()){
			printk("Thread %d :: executed! Counter val: %d\n", thread_no, counter++);
			msleep(thread_no * 1000);
	}

	printk("Thread %d :: finished execution!\n", thread_no);

  return 0;
}

int Thread2(void *data)
{
  unsigned int counter = 0;
	int thread_no = *(int *) data;

	/* Loop to run until the kthread_stop is called */
	while(!kthread_should_stop()){
			printk("Thread %d :: executed! Counter val: %d\n", thread_no, counter++);
			msleep(thread_no * 1000);
	}

	printk("Thread %d :: finished execution!\n", thread_no);

  return 0;
}


/**
* @brief This function is called when the module is loaded into the kernel.
*/

static int __init ModuleInit(void)
{
  printk("Hello World ! \n");

  //Creating Threads.

  thread1_ptr = kthread_create(Thread1, &thread_data1, "Thread 1");

  if(thread1_ptr != NULL)
  {
    /* Start the thread*/
    wake_up_process(thread1_ptr);
		printk("Kernel Thread 1 - is created and is running now!\n");
  }
  else
  {
    printk("Kernel Thread 1 - creation failed!\n");
    return -1;
  }

  thread2_ptr = kthread_create(Thread2, &thread_data2, "Thread 2");

  if(thread2_ptr != NULL)
  {
    /* Start the thread*/
    wake_up_process(thread2_ptr);
		printk("Kernel Thread 2 - is created and is running now!\n");
  }
  else
  {
    printk("Kernel Thread 2 - creation failed!\n");
    return -1;
  }

  /* @Note there is an alternative to kthread_create and wake_up_process
   * Use kthread_run() to create and run the thread.
   * etx_thread = kthread_run(Thread2, &thread_data2, "Thread 2");
  */

  printk("Kernel Thread - creation Successful !\n");


  return 0; //Indicating success.
}

/**
*  @brief This function is called when the module is removed from memory.
*/

static void __exit ModuleExit(void)
{
  kthread_stop(thread1_ptr);
  kthread_stop(thread2_ptr);
  printk("Goodbye Kernel !\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
