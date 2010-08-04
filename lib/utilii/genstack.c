/* This is part of generic stack data structure */

#include <stdio.h>
#include "gendlist.h"
#include "genstack.h"
#include "mem.h"

void stackinit(struct dlist *stack, void (*destroy)(void *data))
{
	dlistinit(stack, destroy);
}

void *stackpush(struct dlist *stack, struct dlistelem *elem)
{
	/* push data on the stack */
	return daddnext(stack, stack->head, elem);
}

void *stackpop(struct dlist *stack, void **data)
{
	/* pop up data from top of stack */
	stack->head = dlistrem(stack, stack->head, data);
	return (void *) stack->head;
}

void stackfree(struct dlist *stack)
{
    dfreeall(stack);
}

void *stackpeek(struct dlist *stack)
{
	return (stack->head == NULL ? NULL : stack->head->data);
}

