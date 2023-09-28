#pragma once

#include <stdbool.h>
#include <stddef.h>

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:  the pointer to the member.
 * @type: the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 */
#define container_of(ptr, type, member)                                        \
	({                                                                     \
		const typeof(((type *)0)->member) *__mptr = (ptr);             \
		(type *)((uint8_t *)__mptr - offsetof(type, member));          \
	})

#define list_entry(ptr, type, member) container_of(ptr, type, member)

struct list_head {
	struct list_head *prev, *next;
};

#define LIST_HEAD_INIT(list)                                                   \
	{                                                                      \
		&(list), &(list)                                               \
	}

void list_add(struct list_head *prev, struct list_head *new);

void list_remove(struct list_head *elem);

bool list_empty(struct list_head *h);