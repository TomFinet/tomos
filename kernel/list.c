#include <list.h>

void list_add(struct list_head* prev, struct list_head* new) {
    struct list_head* next = prev->next;
    prev->next = new;
    new->prev = prev;
    new->next = next;
    next->prev = new;
}