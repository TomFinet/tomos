#pragma once

#include <stddef.h>

#include <memory/kslab.h>

/* Allocate nbytes. */
void *kmalloc(size_t nbytes);

/* Free the object */
void kfree(void *objp);