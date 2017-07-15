/*
 * mm_alloc.h
 *
 * A clone of the interface documented in "man 3 malloc".
 */

#pragma once

#include <stdlib.h>
#include <stdbool.h>

void *mm_malloc(size_t size);
void *mm_realloc(void *ptr, size_t size);
void mm_free(void *ptr);


struct block{
	size_t size;
	bool free;
	struct block *next;
	struct block *prev;
	void* ptr;
	char data[0];
	};
	
typedef struct block *block_ptr;
