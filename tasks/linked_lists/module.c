#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "stack.h"
#include "assert.h"

static int __init test_stack(void)
{
    LIST_HEAD(data_stack);
    stack_entry_t *tos = NULL;
    const char *tos_data = NULL;
    const char* test_data[] = { "1", "2", "3", "4" };
    long i = 0;

    pr_alert("Testing basic stack");

    for (i = 0; i != ARRAY_SIZE(test_data); ++i) {
        stack_push(&data_stack,
                   create_stack_entry((void*)test_data[i])
        );
    }

    for (i = ARRAY_SIZE(test_data) - 1; i >= 0; --i) {
        tos = stack_pop(&data_stack);
        tos_data = STACK_ENTRY_DATA(tos, const char*);
        delete_stack_entry(tos);
        printk(KERN_ALERT "%s == %s\n", tos_data, test_data[i]);
        assert(!strcmp(tos_data, test_data[i]));
    }

    assert(stack_empty(&data_stack));
    return 0;
}

static int __init print_processes_backwards(void)
{
    int code = 0;
    struct task_struct *tsk;
    LIST_HEAD(log_stack);
    stack_entry_t *se = NULL;
    for_each_process(tsk) {
        char *buf = kmalloc(sizeof(tsk->comm), GFP_KERNEL);
        buf = get_task_comm(buf, tsk);
        if (!buf) {
            code = -ENOMEM;
            break;
        }
        se = create_stack_entry((void*)buf);
        if (!se) {
            code = - ENOMEM;
            break;
        }
        stack_push(&log_stack, se);
    }
    while (!stack_empty(&log_stack))
    {
        se = stack_pop(&log_stack);
        const char *buf = STACK_ENTRY_DATA(se, const char*);
        delete_stack_entry(se);
        printk(KERN_ALERT "%s\n", buf);
        kfree(buf);
    }
    return code;
}

static int __init ll_init(void)
{
    printk(KERN_ALERT "Hello, linked_lists\n");
    int code = 0;
    code = test_stack();
    if (!code)
        code = print_processes_backwards();
    return code;
}

static void __exit ll_exit(void)
{
    printk(KERN_ALERT "Goodbye, linked_lists!\n");
}

module_init(ll_init);
module_exit(ll_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked list exercise module");
MODULE_AUTHOR("Kernel hacker!");
