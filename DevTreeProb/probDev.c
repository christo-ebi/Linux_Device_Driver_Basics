#include<linux/module.h>
#include<linux/fs.h>
#include <linux/mod_devicetable.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include<linux/init.h>
#include<linux/cdev.h>

#define MINOR_NUMBER 11   //Minor Number requested.
#define MINOR_NUM_COUNT 1 //Number of Minor Number count.
#define DRIVER_NAME "probDev" //Name for the driver.
#define DRIVER_CLASS "myprobDevClass" //Name for the driver class.

#define BUFFER_SIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("A Linux Kernel Module Example for Probing driver details from Device Tree.");

int my_probe(struct platform_device *plat_dev);

int my_remove(struct platform_device *plat_dev);

static struct of_device_id my_driver_ids[] = {
	{
		.compatible = "custom,myPWM",
	}, { /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, my_driver_ids);


static struct platform_driver my_driver ={
 .probe = my_probe,
 .remove = my_remove,
 .driver = {
        .name = DRIVER_NAME,
        .of_match_table = my_driver_ids,
    }
};

static int __init ModuleInit(void)
{

    printk(KERN_DEBUG "%s :: Init->Entry !",DRIVER_NAME);
	if(platform_driver_register(&my_driver))
    {
        printk(KERN_DEBUG "%s :: Plat-Reg Failed !",DRIVER_NAME);
        return -1;
    }
    printk(KERN_DEBUG "%s :: Init->Exit !",DRIVER_NAME);
    return 0;
}

static void __exit ModuleExit(void)
{
    printk(KERN_DEBUG "%s :: De-Init->Entry !",DRIVER_NAME);
    platform_driver_unregister(&my_driver);
    printk(KERN_DEBUG "%s :: De-Init->Exit !",DRIVER_NAME);
}

int my_probe(struct platform_device *plat_dev)
{
    printk(KERN_DEBUG "%s :: Probe !",DRIVER_NAME);

    struct device *dev = &plat_dev->dev;
    const char *label;
    int my_value;

    if(!device_property_present(dev,"label"))
    {
        printk(KERN_DEBUG "%s ::Label found !",DRIVER_NAME);
        return -1;
    }

    if(!device_property_present(dev,"my_value"))
    {
        printk(KERN_DEBUG "%s ::my_value found !",DRIVER_NAME);
        return -1;
    }

    if(device_property_read_string(dev, "label",&label))
    {
     printk(KERN_DEBUG "%s :: label read failed !",DRIVER_NAME);
    }

    printk(KERN_DEBUG "%s ::Label: %s",DRIVER_NAME,label);

    if(device_property_read_u32(dev,"my_value", &my_value))
    {
     printk(KERN_DEBUG "%s :: my_value read failed !",DRIVER_NAME);
    }

    printk(KERN_DEBUG "%s ::Label: %d",DRIVER_NAME,my_value);

    return 0;
}

int my_remove(struct platform_device *plat_dev)
{
    printk(KERN_DEBUG "%s :: Probe -> Remove !",DRIVER_NAME);
    return 0;
}

module_init(ModuleInit);
module_exit(ModuleExit);
