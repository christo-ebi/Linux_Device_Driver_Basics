#include<linux/module.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>

#define MINOR_NUMBER_RANGE 0      //Minor Number Range.
#define MINOR_NUM_COUNT 1         //Number of Minor Number count.
#define DRIVER_NAME "AutoDevNo" //Name for the driver.
#define DRIVER_CLASS "AutoDevNo" //Name for the driver class.

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("A Linux Kernel Module Example for auto /dev/AutoDevNo file creation.");

//Device struct to store device info;
struct device *dev = NULL;
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

/**
 * @brief This function is called when the device file is opened.
 */

static int driver_open(struct inode *device_file, struct file *instance)
{
  printk(KERN_DEBUG "%s :: fops->Open !",DRIVER_NAME);
  return 0;
}

/**
 * @brief This function is called when the device file is closed.
 */

static int driver_close(struct inode *device_file, struct file *instance)
{
  printk(KERN_DEBUG "%s :: fops->close !",DRIVER_NAME);
  return 0;
}


static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = driver_open,
  .release = driver_close
};

static int __init ModuleInit(void)
{
  printk(KERN_DEBUG "%s :: Init->Entry !",DRIVER_NAME);

  /* register character device and dynamically allocate major number -> alloc_chrdev_region() */
  /*
  * Note:
  * Use MAJOR & MINOR macros to get the major and minor versions when dynamically
  * allocated. Refer: kdev_t.h
  * 
  * alloc_chrdev_region() - register a range of char device numbers
  * @dev: output parameter for first assigned number
  * @baseminor: first of the requested range of minor numbers
  * @count: the number of minor numbers required
  * @name: the name of the associated device or driver
  * 
  * If @major == 0 this functions will dynamically allocate a major and return
  * its number. 
  *  
  * If @major > 0 this function will attempt to reserve a device with the given
  * major number and will return zero on success.
  * 
  * Returns a -ve errno on failure.
  *
  * The name of this device has nothing to do with the name of the device in
  * /dev. It only helps to keep track of the different owners of devices. If
  * your module name has only one type of devices it's ok to use e.g. the name
  * of the module here.
  * 
  * The driver name will show up in the lsmod
  * 
  */
  
  // Request minor number with range 0 = [0-0] with 1 number of minor number.
  //Creates an entry in /sys/module/myCustomPWM

  if(alloc_chrdev_region(&my_device_nr, MINOR_NUMBER_RANGE, MINOR_NUM_COUNT, DRIVER_NAME) > -1 )
  {
    printk(KERN_DEBUG "%s :: myDevNum - registered Device Major Version: %d, Minor Version: %d \n",DRIVER_NAME,MAJOR(my_device_nr), MINOR(my_device_nr));
  }
  else
  {
    printk(KERN_ERR "%s :: myDevNum - could not register device number !\n",DRIVER_NAME);
    return -1; //Indicating failure.
  }

  /*Create device class in /sys/class/myPWMClass*/
  if((my_class = class_create(DRIVER_CLASS)) == NULL)
  {
    printk(KERN_ERR "%s :: Device class not created !\n",DRIVER_NAME);
    goto ClassError;
  }

  /* creates a device and registers it with sysfs will show up in /sys/dev/char/ with major and minor number eg. /sys/dev/char/239\:0/*/
  if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL)
  {
    printk(KERN_ERR "%s ::Can not create device file in sysfs !\n",DRIVER_NAME);
    goto FileError;
  }

  /* initialize a cdev structure */
  cdev_init(&my_device, &fops);

  /* Add character device to system and creates char dev file /dev/myCustomPWM */
  if(cdev_add(&my_device, my_device_nr,1) == -1)
  {
    printk(KERN_ERR "%s ::Kernel failed registering device !\n",DRIVER_NAME);
    goto AddError;
  }
  
  return 0; //Indicating succes.

  AddError:
    device_destroy(my_class, my_device_nr);

  FileError:
    class_destroy(my_class);
  
  ClassError:
    unregister_chrdev_region(my_device_nr,1);
  
  return -1;
}

static void __exit ModuleExit(void)
{
  printk(KERN_DEBUG "%s :: Eixt->Entry !",DRIVER_NAME);
  cdev_del(&my_device);
  device_destroy(my_class, my_device_nr);
  class_destroy(my_class);
  unregister_chrdev_region(my_device_nr,1);
  printk(KERN_DEBUG "%s :: Eixt->GoodBye Kernel !",DRIVER_NAME);
}

//Entry Point into the Kernel Module.
module_init(ModuleInit);
//Exit Kernel Module.
module_exit(ModuleExit);
