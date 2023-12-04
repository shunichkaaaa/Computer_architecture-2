#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Dariia Hryhorieva <gdaria296@gmail.com>");
MODULE_DESCRIPTION("Hello2 module using print_hello from Hello1");
MODULE_LICENSE("Dual BSD/GPL");

// Add a global variable for the parameter
static unsigned int hello_number = 1;

// Register the parameter
module_param(hello_number, uint, 0444);
MODULE_PARM_DESC(hello_number, "Number of times to print \"Hello, world!\"");

static int __init hello2_init(void)
{
	ktime_t before, after;

	before = ktime_get();

	// Call the print_hello function from hello1
	print_hello(hello_number);

	after = ktime_get();

	pr_info("Print duration in hello2: %lld ns\n", ktime_to_ns(ktime_sub(after, before)));

	return 0;
}

static void __exit hello2_exit(void)
{
	exit_hello();
}

module_init(hello2_init);
module_exit(hello2_exit);

