/* This routine is part of generic double linked list structure */
#include "gendlist.h"
#include "mem.h"
#include "assert.h"

void dlistinit(struct dlist *list, void (*destroy) (void *data))
{
	if (list == NULL)
		NEW(list);

	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	list->destroy = destroy;
}

void *dnewitem(const void *data)
{
        /* new element */
        struct dlistelem *elem;

        NEW(elem);
        elem->data = (void *) data;
        elem->next = NULL;
	elem->prev = NULL;

        return (void *) elem;
}

void *daddprev(struct dlist *list, struct dlistelem *before, struct dlistelem *elem) 
{
	if (list->size == 0) {
		list->head = elem;
		list->tail = elem;
	} else {
		elem->next = before;
		elem->prev = before->prev;
		if (before->prev == NULL)
			list->head = elem;
		else 
			before->prev->next = elem;

		before->prev = elem;
	}
	
	list->size++;
	return (void *) elem;
	
}

void *daddnext(struct dlist *list, struct dlistelem *after, struct dlistelem *elem)
{
	struct dlistelem *p;
	
	if (list->size == 0) {
		list->head = elem;
		list->head->prev = NULL;
		list->head->next = NULL;
		list->tail = elem;
	} else {
		elem->prev = after;
		elem->next = after->next;
		if (after->next == NULL)	
			list->tail = elem;
		else
			after->next->prev = elem;
		
		after->next = elem;
	
    }
	
	list->size++;
	return (void *)list->head;

}

void *dlookup(struct dlist *list, const void *data,
			  int (*cmp)(const void *, const void *))
{
   struct dlistelem *p;
   for (p = list->head; p != NULL; p = p->next)
	   if ( (*cmp)(data, p->data) == 0 )
            return (void *) p;
   return NULL; /* no match */
}

void dapply(struct dlist *list, void (*fn) (struct dlistelem *, void *), void *argv)
{
   struct dlistelem *p;
   for (p = list->head; p != NULL; p = p->next)
        (*fn)(p, argv); /* call the function */

}


void dfreeall(struct dlist *list)
{
	struct dlistelem *next;

    for ( ; list->head != NULL; list->head = next) {
        next = list->head->next;
        list->destroy(list->head->data);
        FREE(list->head);
    }

}

void *ddelitem(struct dlist *list, const void *data,
               int (*cmp)(const void *, const void*))
{
    struct dlistelem *p;
    struct dlistelem *elem;

    elem = NULL;
    for (p = list->head; p != NULL; p = p->next) {
        if((*cmp)(data, p->data) == 0) {
            if (elem == NULL) {
                list->head = p->next;
				if (list->head == NULL)
					list->tail = NULL;
				else
					p->next->prev = NULL;
            } else {
                elem->next = p->next;
				if (p->next == NULL)
					list->tail = p->prev;
				else
					p->next->prev = elem; 
			}

            FREE(p);
			list->size--;
            return (void *) list->head;
        }
        elem = p;
    }
    return NULL; /* can't get here */

}

void *dlistrem(struct dlist *list, struct dlistelem *elem, void **data)
{
	if (elem == NULL) 
		return NULL;

	assert(*data != NULL);
	*data = elem->data;

	if (elem == list->head) {
		list->head = elem->next;
	
		if (list->head == NULL)
			list->tail = NULL;
		else
			elem->next->prev = NULL;

	} else {

		elem->prev->next = elem->next;
		if (elem->next == NULL)
			list->tail = elem->prev;
		else
			elem->next->prev = elem->prev;

	}
	
	FREE(elem);
	list->size--;
	return (void *) list->head;
}
	



