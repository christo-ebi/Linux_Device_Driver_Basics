#include<linux/module.h>
#include<linux/fs.h>
#include<linux/init.h>
#include<linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>

#define MYMAJOR 90                //Major Number 
#define MINOR_NUMBER 11           //Minor Number requested.
#define MINOR_NUM_COUNT 1         //Number of Minor Number count.
#define DRIVER_NAME "myCustomPWM" //Name for the driver.
#define DRIVER_CLASS "myPWMClass" //Name for the driver class.

#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("A Linux Kernel Module Example for PWM Driver.");

//Device struct to store device info;
struct device *dev = NULL;
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;
static struct pwm_device *myPWMDev;

bool Enable_Flag = false;

int my_probe(struct platform_device *plat_dev);

int my_remove(struct platform_device *plat_dev);

static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "starlight,PWM",
	}, { /* sentinel */ }
};

//Used to auto load the driver when the device tree node with compatible (as above) is detected.
MODULE_DEVICE_TABLE(of, my_driver_ids);

static struct platform_driver my_driver ={
 .probe = my_probe,
 .remove = my_remove,
 .driver = {
        .name = DRIVER_NAME,
        .of_match_table = my_driver_ids,
    }
};

int my_probe(struct platform_device *plat_dev)
{
    printk(KERN_DEBUG "%s :: Probe !",DRIVER_NAME);

    int ret_val;

    //Get the device info from the device tree.
    dev = &plat_dev->dev;

    if(!device_property_present(dev,"pwms"))
    {
        printk(KERN_DEBUG "%s ::my_value not found !",DRIVER_NAME);
        return -1;
    }

    myPWMDev= pwm_get(dev, NULL);

    printk(KERN_DEBUG "%s :: pwm_config :: retVal: %d !",DRIVER_NAME,ret_val);

    if (IS_ERR(myPWMDev)){
        printk("Requesting PWM failed %p", ERR_CAST(myPWMDev));
        return -EIO;
    }

    ret_val = pwm_config(myPWMDev, 250000, 5000000);

    printk(KERN_DEBUG "%s :: pwm_config :: retVal: %d !",DRIVER_NAME,ret_val);

	  if(ret_val<0)
    {
      printk(KERN_DEBUG "%s ::PWM Config failed :: Error:%d !",DRIVER_NAME,ret_val);
      return -1;
    }

    ret_val = pwm_enable(myPWMDev);

    printk(KERN_DEBUG "%s :: pwm_enable :: retVal: %d !",DRIVER_NAME,ret_val);

    if(ret_val<0)
    {
      printk(KERN_DEBUG "%s ::PWM Enable failed :: Error:%d !",DRIVER_NAME,ret_val);
      return -1;
    }

    printk(KERN_DEBUG "%s ::PWM Request Successful !",DRIVER_NAME);

    return 0;
}

int my_remove(struct platform_device *plat_dev)
{
    pwm_disable(myPWMDev);
	  pwm_put(myPWMDev);
    printk(KERN_DEBUG "%s :: Probe -> Remove !",DRIVER_NAME);
    return 0;
}


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

/**
 * @brief Read data from the buffer
 */

static ssize_t driver_read(struct file *File, char *user_buffer,size_t count, loff_t *offs)
{
  printk(KERN_DEBUG "%s :: fops->Read !",DRIVER_NAME);
 
  int to_copy, not_copied, delta;
  char value;

  switch(Enable_Flag)
  {
    case true:{
      value ='1';
      break;
    }
    case false:{
      value ='0';
      break;
    }
  }

  to_copy = min(count, sizeof(value));

  not_copied = copy_to_user((void *)user_buffer, &value, to_copy);

  delta = to_copy - not_copied;

  *offs+=delta;

  if(*offs>to_copy)
  {
    return 0;
  }

  return delta;
}

/**
 * @brief Write data to the buffer.
 */

static ssize_t driver_write(struct file *File,const char *user_buffer,size_t count, loff_t *offs)
{
  printk(KERN_DEBUG "%s :: fops->Write !",DRIVER_NAME);
  
  int to_copy, not_copied, delta;
  char value;

  to_copy = min(count,sizeof(value));
  
  not_copied = copy_from_user(&value, user_buffer, to_copy);

  switch(value)
  {
    case '0':{
      if(pwm_enable(myPWMDev))
      {
        Enable_Flag = false;
      }
      break;
    }

    case '1':{
      pwm_disable(myPWMDev);
      Enable_Flag = true;
      break;
    }

    default:{
      printk(KERN_DEBUG "%s :: Write->Invalid !",DRIVER_NAME);
    }
  }

  delta = to_copy - not_copied;

  return delta;
}

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = driver_open,
  .release = driver_close,
  .read = driver_read,
  .write = driver_write
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
  
  // Request minor number with range [0-0] with 1 number of minor number.
  //Creates an entry in /sys/module/myCustomPWM

  if(alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) > -1 )
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

  if(platform_driver_register(&my_driver))
  {
    printk(KERN_DEBUG "%s :: Plat-Reg Failed !",DRIVER_NAME);
    goto PlatRegError;
  }
  
  return 0; //Indicating succes.

  PlatRegError:
    cdev_del(&my_device);

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
  platform_driver_unregister(&my_driver);
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
