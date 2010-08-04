#line 28 "genlist.nw"
#include<stdio.h>
#include "genlist.h"
#include "mem.h"


#line 46 "genlist.nw"
void glistinit(struct list *list, void (*destroy)(void *data))
{
	/* if new list allocate memory */
	if (list == NULL)
		NEW(list);
	
#line 61 "genlist.nw"
list->size = 0;
list->head = NULL;
list->tail = NULL;
list->destroy = destroy;

#line 52 "genlist.nw"
}
#line 69 "genlist.nw"
void *gnewitem(const void *data)
{
	/* new element */
	struct listelem *elem;
	
	NEW(elem);
	elem->data = (void *) data;
	elem->next = NULL;
	
	return (void *) elem;
}
 
#line 85 "genlist.nw"
void *gaddfront(struct list *list, struct listelem *elem)
{
	elem->next = list->head;
	list->size++; /* adjust size */
	return (void *) elem;
}

#line 95 "genlist.nw"
void *gaddend(struct list *list, struct listelem *elem)
{
	struct listelem *p;
	
	
#line 108 "genlist.nw"
if (list->head == NULL) {
	list->head = elem;
	list->size++;
	return (void *) elem;
}

#line 100 "genlist.nw"
	
#line 118 "genlist.nw"
for (p = list->head; p->next != NULL; p = p->next)
	; /* next element */
p->next = elem;
/* adjust size */
list->size++;


#line 102 "genlist.nw"
	return (void *) list->head;
}

#line 127 "genlist.nw"
void *glookup(struct list *list, const void *data,
		int (*cmp)(const void*, const void*))
{
	struct listelem *p;
	
	
#line 143 "genlist.nw"
for (p = list->head; p != NULL; p = p->next)
	if ( (*cmp)(data, p->data) == 0)
		return (void *) p;


#line 134 "genlist.nw"
	/* no match */
	return NULL;
}

#line 153 "genlist.nw"
void gapply(struct list *list, void (*fn)(struct listelem*, void*), void *arg)
{
	struct listelem *p;

	for (p = list->head; p != NULL; p = p->next) 
		(*fn)(p, arg); /* call function */

}

#line 166 "genlist.nw"
void gfreeall(struct list *list)
{
	struct listelem *next;

	for ( ; list->head != NULL; list->head = next) {
		next = list->head->next;
		list->destroy(list->head->data); /* free data */
		FREE(list->head);
	}
}

#line 183 "genlist.nw"
void *gdelitem(struct list *list, const void *data,
		int (*cmp)(const void*, const void*))
{
	struct listelem *p;
	struct listelem *prev;

	prev = NULL;
	
#line 203 "genlist.nw"
for (p = list->head; p != NULL; p = p->next) {
	if ((*cmp)(data, p->data) == 0) {
		if (prev == NULL) 
			list->head = p->next;
		else
			prev->next = p->next;
		FREE(p);
		/* return list head */
		return (void *) list->head;
	}
	prev = p;
}

#line 191 "genlist.nw"
	
#line 197 "genlist.nw"
return NULL;

#line 192 "genlist.nw"
}



