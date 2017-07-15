/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines.
 */

#include "mm_alloc.h"
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#define METADATA_LENGTH 40

void *base_ptr = NULL;//pointer to the base of the heap

block_ptr heap_increment(block_ptr prev_block, size_t i){
	block_ptr curr_block;
	curr_block = sbrk(0);//current location of break
	int alloc_size = (int) sbrk(METADATA_LENGTH + i);
	if(alloc_size<0)
		return NULL;
	
	curr_block->size = i;
	curr_block->prev = prev_block;
	curr_block->free = false;
	curr_block->ptr = curr_block->data;
	return curr_block;
	}
	
block_ptr coalesce (block_ptr curr_block){
	if(curr_block->next != NULL && curr_block->next->free == true){
		curr_block->size = curr_block->size + curr_block->next->size + METADATA_LENGTH;
		curr_block->next = curr_block->next->next;
		}
	else if(curr_block->prev != NULL && curr_block->prev->free == true){
		curr_block->size = curr_block->size + curr_block->prev->size + METADATA_LENGTH;
		curr_block->prev = curr_block->prev->prev;
		}	
	return curr_block;
	
}
	

void *mm_malloc(size_t size) {
    /* YOUR CODE HERE */
    block_ptr curr_block, prev_block;
    if (base_ptr!= NULL){
		curr_block = base_ptr;
		prev_block = base_ptr;
		while(curr_block->free ==0 && curr_block->size <=size && curr_block!=NULL){
			prev_block = curr_block;
			curr_block = curr_block->next;
			}
		curr_block = heap_increment(prev_block, size);//extend
		if(!curr_block)
			return NULL;
		}
	
	else{
		//Initial stage
		curr_block = heap_increment(NULL,size);//extend
		if(!curr_block)
			return NULL;
		base_ptr = curr_block;
		}
    
    
   // void *allocated_address=sbrk(size);
    return curr_block->data;
}

void *mm_realloc(void *ptr, size_t size) {
	
	
    /* YOUR CODE HERE */
    block_ptr curr_block,new_block;
    void* new_alloc;
    
    curr_block = ptr-METADATA_LENGTH;
    mm_free(ptr);
    if(size == 0)
		return NULL;
    new_alloc = mm_malloc(size);
    if(!new_alloc)
		return NULL;
	new_block = new_alloc - METADATA_LENGTH;
    memcpy(new_block->data, curr_block->data, curr_block->size);

    return new_alloc;
}

void mm_free(void *ptr) {
    /* YOUR CODE HERE */
    block_ptr curr_block;
    if(ptr==NULL)
		return;
	else{
		curr_block = ptr - METADATA_LENGTH;
		curr_block->free = true;
		if(curr_block->next!=NULL && curr_block->next->free==1)
			coalesce(curr_block);
		brk(curr_block);
		}
}
