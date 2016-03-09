#include <linux/slab.h>
#include <linux/gfp.h>

#include "stack.h"

stack_entry_t* create_stack_entry(void *data)
{
    stack_entry_t* se = kmalloc(sizeof(stack_entry_t), GFP_KERNEL);
    if (se)
        se->data = data;
    return se;
}

void delete_stack_entry(stack_entry_t *entry)
{
    kfree(entry);
}

void stack_push(struct list_head *stack, stack_entry_t *entry)
{
    list_add(&entry->lh, stack);
}

stack_entry_t* stack_pop(struct list_head *stack)
{
    if (stack_empty(stack))
        return NULL;
    stack_entry_t* se = list_first_entry(stack, stack_entry_t, lh);
    list_del(stack->next);
    return se;
}
