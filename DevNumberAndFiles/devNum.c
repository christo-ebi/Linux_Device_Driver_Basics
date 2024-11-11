#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("A Linux Kernel Module Example for device numbers.");

/**
 * @brief This function is called when the device file is opened.
 */

static int driver_open(struct inode *device_file, struct file *instance)
{
  printk("Device Number example: - open was called");
  return 0;
}

/**
 * @brief This function is called when the device file is closed.
 */

static int driver_close(struct inode *device_file, struct file *instance)
{
  printk("Device Number example: - close was called");
  return 0;
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = driver_open,
  .release = driver_close
};

#define MYMAJOR 90

/**
* @brief This function is called when the module is loaded into the kernel.
*/

static int __init ModuleInit(void)
{
  printk("Hello World ! \n");
  int retval;
  /* register device here. */
  retval = register_chrdev(MYMAJOR, "myDevNum", &fops);
  if(retval == 0)
  {
    printk("myDevNum - registered Device Major Version: %d, Minor %d \n",MYMAJOR, 0);
  }
  else if(retval > 0)
  {
    printk("myDevNum - registered Device Major Version: %d, Minor Version: %d \n",retval>>20, retval&0xfffff);
  }
  else{
    printk("myDevNum - could not register device number !\n");
  }
  return 0; //Indicating success.
}

/**
*  @brief This function is called when the module is removed from memory.
*/

static void __exit ModuleExit(void)
{
  unregister_chrdev(MYMAJOR, "myDevNum");
  printk("Goodbye Kernel !\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
