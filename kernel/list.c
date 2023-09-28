#include <list.h>

void list_add(struct list_head *prev, struct list_head *new)
{
	if (prev == NULL || new == NULL) {
		return;
	}
	struct list_head *next = prev->next;
	prev->next = new;
	new->prev = prev;
	new->next = next;
	next->prev = new;
}

void list_remove(struct list_head *elem)
{
	struct list_head *prev = elem->prev;
	struct list_head *next = elem->next;

	elem->prev = NULL;
	elem->next = NULL;

	if (prev) {
		prev->next = next;
	}

	if (next) {
		next->prev = prev;
	}
}

bool list_empty(struct list_head *h)
{
	return h->next == h;
}