/* This is header file for generic linked list data structure */

#ifndef _DLIST_H_
#define _DLIST_H_

#include <stdlib.h>

struct dlistelem {
	void *data;
	struct dlistelem *next; /* in list */
	struct dlistelem *prev; /* in list */
};

struct dlist {
	int size;
	int (*match) (const void *key1, const void *key2);
	void (*destroy) (void *data);
	
	struct dlistelem *head;
	struct dlistelem *tail;
};

/* public interface */
void dlistinit(struct dlist *list, void (*destroy)(void *data));
int (*cmp)(const void*, const void*);
void *daddprev(struct dlist *list, struct dlistelem *before, struct dlistelem *elem);
void *dnewitem(const void *data);
void *daddnext(struct dlist *list, struct dlistelem *after, struct dlistelem *elem);
void *dlookup(struct dlist *list, const void *data,
		     int (*cmp)(const void*, const void*));
void dapply(struct dlist *list, void (*fn)(struct dlistelem*, void*),
		    void *arg);
void dfreeall(struct dlist *list);
void *ddelitem(struct dlist *list, const void *data,
			int (*cmp)(const void*, const void*));
void *dlistrem(struct dlist *list, struct dlistelem *elem, void **data);
#endif


	
