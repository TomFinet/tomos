#pragma once

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:  the pointer to the member.
 * @type: the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 */
#define container_of(ptr, type, member) ({              \
    const typeof( ((type *) 0)->member ) mptr = (ptr);  \
    (type *)(mptr - offsetof(type, member)); })

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member) 

struct list_head {
    struct list_head *prev, *next;
};

inline void list_head_init(struct list_head* list) {
    list->next = list;
    list->prev = list;
}

void list_add(struct list_head* prev, struct list_head* new);