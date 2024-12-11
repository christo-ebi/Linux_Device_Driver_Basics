#include <linux/module.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("A Linux Kernel Module example with parametes");

/**
* @brief This function is called when the module is loaded into the kernel.
*/
static unsigned int number = 1;
static char *name="Sample";

module_param(name, charp, 0);
module_param(number, uint, 0);

MODULE_PARM_DESC(name, "Device name to use in this Kernel Module");
MODULE_PARM_DESC(number, "A number to be used in Kernel Module (Eg GPIO No)");

static int __init ModuleInit(void)
{
  printk("Number = %u\n", number);
	printk("Name = %s\n", name);
  printk("Hello World ! \n");
  return 0; //Indicating success.
}

/**
*  @brief This function is called when the module is removed from memory.
*/

static void __exit ModuleExit(void)
{
  printk("Goodbye Kernel !\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
