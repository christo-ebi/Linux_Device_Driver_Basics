#include <linux/module.h>
#include <linux/init.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CHRIS");
MODULE_DESCRIPTION("A hello world Linux Kernel Module");

/**
* @brief This function is called when the module is loaded into the kernel.
*/

static int __init ModuleInit(void)
{
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
