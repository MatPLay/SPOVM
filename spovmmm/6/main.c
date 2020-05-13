#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define align(x) (( (((x) - 1) >> 2) << 2) + 4)
#define BLOCK_SIZE 20		/* align(4*4 + 1)    */

#define TRUE 1
#define FALSE 0

void* base = NULL;

typedef struct heap_block *block;
struct heap_block
{
  size_t size;
  block next;
  block previous;
  int32_t free;
  void* ptr;
  int8_t separator[1];
};

void* mihalloc(size_t size);
block find_block(block* last, size_t size);
block extend_heap(block last, size_t size);
void split_block(block b, size_t size);
void* kalloc(size_t number, size_t size);
block merge(block fused);
block get_block(void *ptr);
int valid_address(void *ptr);
void free(void* ptr);
void copy_block(block source, block destination);
void* realloc(void* ptr, size_t size);

#define ar1 24
#define ar2 16

int main()
{
  int* array = (int*) mihalloc(ar1*sizeof(int));

  for (int i = 0 ; i < ar1; i++) {

    array[i] = i;
    
  }

  for (int i = 0 ; i < ar1; i++) {

    printf("%d ", array[i]);
    
  }

  printf("\n");
  
  free(array);

  int* array2 = (int*) realloc(array, ar2*sizeof(int));

  for (int i = 0 ; i < ar2; i++) {

    array2[i] = -i;
    
  }

  for (int i = 0 ; i < ar1; i++) {

    printf("%d ", array2[i]);
    
  }
  

  
  return 0;
}


void* mihalloc(size_t size)
{
  block alloc_block = NULL, last = NULL;

  size_t alloc_size = align(size);

  if(NULL != base)
    {
      last = base;
      
      alloc_block = find_block(&last, alloc_size);

      if(NULL != alloc_block)
	{
	  if((alloc_block->size - alloc_size) >= BLOCK_SIZE + 4)
	    {
	      split_block(alloc_block, alloc_size);
	    }
	  alloc_block->free = FALSE;
	}
      else
	{
	  alloc_block = extend_heap(last, alloc_size);
	  
	  if(NULL == alloc_block)
	    {
	      return NULL;
	    }
	}
    }
  else
    {
      alloc_block = extend_heap(NULL, alloc_size);
      
      if(NULL == alloc_block)
	{
	  return NULL;
	}

      base = alloc_block;
      
    }

  return alloc_block->separator;
}

block find_block(block* last, size_t size)
{
  block suitable = base;
  
  while(NULL != suitable && ((suitable->free != TRUE) || suitable->size < size))
    {
      *last = suitable;
      suitable = suitable->next;
    }
  
  return suitable;
}

block extend_heap(block last, size_t size)
{
  block new_break = NULL;

  new_break = sbrk(0);

  int moved_size = (int) sbrk(BLOCK_SIZE + size);

  if(moved_size < 0)
    {
      return NULL;
    }

  new_break->size = size;
  new_break->next = NULL;
  new_break->previous = last;
  new_break->ptr = new_break->separator;
  
  if(NULL != last)
    {
      last->next = new_break;
    }

  new_break->free = FALSE;

  return new_break;
}

void split_block(block b, size_t size)
{
  block new_block = NULL;

  new_block = (block) b->separator + size;
  new_block->size = b->size - size - BLOCK_SIZE;
  new_block->next = b->next;
  new_block->previous = b;
  new_block->free = TRUE;

  new_block->ptr = new_block->separator;

  b->size = size;
  b->next = new_block;

  if(NULL != new_block->next)
    {
      new_block->next->previous = new_block;
    }
  
}

void* kalloc(size_t number, size_t size)
{
  size_t* new_block = NULL;
  size_t alligned_size;

  new_block = mihalloc(number * size);

  if(NULL != new_block)
    {
      alligned_size = align(number * size) << 2; /* to return new_block address instead of new_block */
                                                 /* plus struct size + 1 byte */
      for(size_t i; i < alligned_size; i++)
	{
	  new_block[i] = 0;
	}
    }
  
  return new_block;
}

block merge(block fused)
{
  if(NULL != fused->next && TRUE == fused->next->free)
    {
      fused->size += BLOCK_SIZE + fused->next->size;
      fused->next = fused->next->next;

      if(NULL != fused->next)
	{
	  fused->next->previous = fused;
	}
    }
  return fused;
}

block get_block(void *ptr)
{
  int8_t* temp = NULL;
  
  temp = ptr;

  return ptr = temp -= BLOCK_SIZE;
}

int valid_address(void* ptr)
{
  if(NULL != base)
    {
      if(ptr > base && ptr < sbrk(0))
	{
	  return ptr == (get_block(ptr))->ptr;
	}
    }
  return FALSE;
}

void free(void* ptr)
{
  block freed_block = NULL;

  if(valid_address(ptr))
    {
      freed_block = get_block(ptr);
      freed_block->free = TRUE;

      if(NULL != freed_block->previous && TRUE == freed_block->previous->free)
	{
	  freed_block = merge(freed_block->previous);
	}

      if(NULL != freed_block->next)
	{
	  merge(freed_block);
	}
      else
	{
	  /* end of heap case */
	  if(NULL != freed_block->previous)
	    {
	      freed_block->previous->next = NULL;
	    }
	  else
	    {
	      base = NULL;
	    }
	  brk(freed_block);
	}
    }
}


void copy_block(block source, block destination)
{
  int32_t* source_data = NULL;
  int32_t* destination_data = NULL;

  source_data = source->ptr;
  destination_data = destination->ptr;

  for(size_t i = 0; i*4 < source->size && i*4 < destination->size; i++)
    {
      destination_data[i] = source_data[i];
    }
}


void* realloc(void* ptr, size_t size)
{
  size_t realloc_size;
  block old_block = NULL, new_block = NULL;

  void* new_ptr = NULL;

  if(NULL == ptr)
    {
      return mihalloc(size);
    }

  if(valid_address(ptr))
    {
      realloc_size = align(size);
      old_block = get_block(ptr);

      if(old_block-> size >= realloc_size)
	{
	  if(old_block->size - realloc_size >= (BLOCK_SIZE + 4))
	    {
	      split_block(old_block, realloc_size);
	    }
	}
      else
	{
	  if(NULL != old_block->next &&
	     TRUE == old_block->next->free &&
	     (old_block->size + BLOCK_SIZE + old_block->next->size) >= realloc_size)
	    {
	      merge(old_block);

	      if(old_block->size - realloc_size >= (BLOCK_SIZE + 4))
		{
		  split_block(old_block, realloc_size);
		}
	    }
	  else
	    {
	      new_ptr = mihalloc(realloc_size);

	      if(NULL == new_ptr)
		{
		  return NULL;
		}

	      new_block = get_block(new_ptr);

	      printf("queque");
	      
	      copy_block(old_block, new_block);

	      free(ptr);

	      return new_block;
	      
	    }

	}

      return ptr;
    }
  
  return NULL;
}
