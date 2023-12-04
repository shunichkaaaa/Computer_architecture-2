
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Dariia Hryhorieva <gdaria296@gmail.com>");
MODULE_DESCRIPTION("Hello1 module with print_hello and exit_hello functions");
MODULE_LICENSE("Dual BSD/GPL");

// Declare data structure
struct my_data {
	struct list_head list;
	ktime_t timestamp;
};

// Static variable for the list head
static LIST_HEAD(my_list);

int print_hello(unsigned int count)
{
	int i;
	struct my_data *new_data;

	if (count == 0 || (count > 5 && count < 10)) {
		pr_warn("Warning: hello_number is set to %u.\n", count);
	} else if (count > 10) {
		pr_err("Invalid value for hello_number. Module initialization failed.\n");
		BUG_ON(1);  // Trigger a BUG_ON for invalid value
	}

	// Add the structure to the list, and print "Hello, world!"
	for (i = 0; i < count; i++) {

		// Simulate kmalloc() returning 0 for the last element
		if (i == count - 1) {
			new_data = NULL;
		} else {
			new_data = kmalloc(sizeof(struct my_data), GFP_KERNEL);
		}

		if (!new_data) {
			pr_err("Failed to allocate memory\n");
			return -ENOMEM;
		}

		INIT_LIST_HEAD(&new_data->list);
		new_data->timestamp = ktime_get();
		list_add_tail(&new_data->list, &my_list);

		pr_info("Hello, world!\n");
	}

	return 0;
}
EXPORT_SYMBOL(print_hello);

void exit_hello(void)
{
	struct my_data *entry, *temp;

	// Traverse the list, print the time,
	// delete the element, and free memory
	list_for_each_entry_safe(entry, temp, &my_list, list) {
		pr_info("Timestamp: %lld ns\n", ktime_to_ns(entry->timestamp));

		list_del(&entry->list);
		kfree(entry);
	}
}
EXPORT_SYMBOL(exit_hello);
