/* This is header file for generic stack data structure */

#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>

/* public interface */
void stackinit(struct dlist *stack, void (*destroy)(void *data));
void *stackpush(struct dlist *stack, struct dlistelem *elem);
void *stackpop(struct dlist *stack, void **data);
void stackfree(struct dlist *stack);
void *stackpeek(struct dlist *stack);

#endif 

