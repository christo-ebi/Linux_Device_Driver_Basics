#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("A Linux Kernel Module Example for read and write.");

/* Buffer for data */
static char buffer[255];
static unsigned int buffer_pointer;

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "ReadWriteDriver"
#define DRIVER_CLASS "MyModuleClass"


/**
 * @brief Read data from the buffer
 */

static ssize_t driver_read(struct file *File, char *user_buffer,size_t count, loff_t *offs){
int to_copy, not_copied, delta;

  /*Get amount of data to copy */
  to_copy = min(count, buffer_pointer);

  /* Copy data to user */
  not_copied = copy_to_user(user_buffer, buffer, to_copy);
  
  /* calculate data */
  delta = to_copy - not_copied;

  return delta;

}

/**
 * @brief Write data to the buffer.
 */

static ssize_t driver_write(struct file *File,const char *user_buffer,size_t count, loff_t *offs){
int to_copy, not_copied, delta;

  /*Get amount of data to copy */
  to_copy = min(count, sizeof(buffer));

  /* Copy data to user */
  not_copied = copy_from_user(buffer, user_buffer, to_copy);
  buffer_pointer = to_copy;
  /* calculate data */
  delta = to_copy - not_copied;

  return delta;

}


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
  .release = driver_close,
  .read = driver_read,
  .write = driver_write
};

#define MYMAJOR 90

/**
* @brief This function is called when the module is loaded into the kernel.
*/

static int __init ModuleInit(void)
{
  printk("Hello World ! \n");

  /* Allocate a device number here. */
  if(alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME)< 0)
  {
    printk("Device Number could not be allocated! \n ");
  }

  printk("Read -Write Example Major No: %d, Mior No: %d was registered ", my_device_nr >> 20, my_device_nr & 0xfffff);

  /*Create device class */

  if((my_class = class_create(DRIVER_CLASS)) == NULL)
  {
    printk("Device class not created! \n");
    goto ClassError;
  }

  /* Create device file*/
  if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL)
  {
    printk("Can not create device file !\n");
    goto FileError;
  }
  
  /* Initialize device files */
  cdev_init(&my_device, &fops);

  if(cdev_add(&my_device, my_device_nr,1) == -1)
  {
    printk("Kernel failed registering device ! \n");
    goto AddError;
  }
  
  return 0; //Indicating success.
  
  AddError:
    device_destroy(my_class, my_device_nr);

  FileError:
    class_destroy(my_class);
  
  ClassError:
    unregister_chrdev_region(my_device_nr,1);
  
  return -1;
}

/**
*  @brief This function is called when the module is removed from memory.
*/

static void __exit ModuleExit(void)
{
  cdev_del(&my_device);
  device_destroy(my_class, my_device_nr);
  class_destroy(my_class);
  unregister_chrdev_region(my_device_nr,1);
  printk("Goodbye Kernel !\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
