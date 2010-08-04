/* This is header file for generic list data structure */

#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>

struct listelem {
	void *data;
	struct listelem *next; /* in list */
};

struct list {
	int size;
	int (*match) (const void *key1, const void *key2);
	void (*destroy) (void *data);
	
	struct listelem *head;
	struct listelem *tail;
};

/* public interface */
void glistinit(struct list *list, void (*destroy)(void *data));
//int (*cmpfun)(const void*, const void*);
void *gaddfront(struct list *list, struct listelem *elem);
void *gnewitem(const void *data);
void *gaddend(struct list *list, struct listelem *elem);
void *glookup(struct list *list, const void *data,
		     int (*cmpfun)(const void*, const void*));
void gapply(struct list *list, void (*fn)(struct listelem*, void*),
		    void *arg);
void gfreeall(struct list *list);
void *gdelitem(struct list *list, const void *data,
			int (*cmpfun)(const void*, const void*));

#endif


	
