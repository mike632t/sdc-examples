/*
 * sdc-malloc.c
 *
 * Copyright(C) 2026   MT
 *
 * Provides support for malloc() and free().
 * 
 * Allocates  the required number of blocks of memory from the free list to 
 * hold the number of bytes requested plus an additional block that is used 
 * to store the size of the block and the link to the next block.
 * 
 * Memory  is allocated from the free list by searching for the first  free 
 * contiguous chunk large enough to hold the number of bytes requested.
 *
 * When blocks are freed they are returned to the free list and merged with 
 * any physically adjacent free blocks to prevent fragmentation.
 *
 * This  program is free software: you can redistribute it and/or modify it
 * under  the terms of the GNU General Public License as published  by  the
 * Free  Software Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 20 Oct 26   0.1   - Initial version - MT
 * 
 * ToDo              - Figure out where to define these functions!
 *
 */

#include <stdlib.h>

#define HEAP_UNIT_SHIFT  3
#define HEAP_UNIT_SIZE   (1 << HEAP_UNIT_SHIFT)

typedef struct heap_extent leaf;

struct heap_extent {
   leaf *next;
   unsigned int blocks;
};

leaf *__sdcc_free_list = NULL;  /* Pointer to the free list */
leaf *__sdcc_modified_list = NULL;  /* Pointer to the modified list (list of modified memory blocks) */

extern char __sdcc_heap_start[];
extern char __sdcc_heap_end[];

static unsigned char __sdcc_heap_initialized = 0;

/*
 * heap_init()
 * 
 * Initializes the heap and creates an empty modified list and a free  list 
 * with a single chunk containing all the free space on the heap.
 * 
 */

void __sdcc_heap_init(void)
{
   unsigned int i_start;
   unsigned int i_end;
    
   /* Get the raw literal addresses of the labels */
   i_start = (unsigned int)&__sdcc_heap_start;
   i_end   = (unsigned int)&__sdcc_heap_end;
   /* Align boundaries to unit block sizes */
   i_start = (i_start + (HEAP_UNIT_SIZE - 1)) & ~(unsigned int)(HEAP_UNIT_SIZE - 1);  /* Round up to block boundary */
   i_end   = i_end & ~(unsigned int)(HEAP_UNIT_SIZE - 1);  /* Round down to block boundary */

   __sdcc_free_list = NULL;
   __sdcc_modified_list = NULL;

   if (i_end > i_start)
   {
      unsigned int i_heap_size = (unsigned int)((i_end - i_start) >> HEAP_UNIT_SHIFT);  /* Compute total pool blocks */
      if (i_heap_size >= 2)
      {
         leaf *h_block = (leaf *)i_start;  /* Set up initial giant free block */
         h_block->next = NULL;  /* Terminal list point */
         h_block->blocks = i_heap_size;  /* Assure size records whole space */
         __sdcc_free_list = h_block;  /* Assign block as head of free list */
      }
   }
   __sdcc_heap_initialized = 1;  /* Mark initialization phase complete */
}

/*
 * malloc()
 * 
 * Memory  is allocated from the free list by searching for the first  free 
 * contiguous chunk large enough to hold the number of bytes requested.
 * 
 * Each  chunk  will hold the required number of blocks needed to hold  the
 * number of bytes requested plus an additional block that is used to store 
 * the size of the chunk and a link to the next chunk.  
 * 
 * If when allocating blocks from the free list the remaining space is less 
 * than  two blocks (the minimum size for a chunk being one block data  and 
 * one  block for the descriptor) then the extra block is allocated to  the
 * new chunk to minimize fragmentation.
 * 
 */
 
void *malloc(size_t t_size)
{
   leaf *h_block;
   leaf *h_new_extent;
   leaf **h_link;
   unsigned int i_data_blocks;
   unsigned int i_needed_blocks;
   unsigned int i_remainder;

   if (!__sdcc_heap_initialized)
      __sdcc_heap_init();  /* Auto-initialize heap structure on first execution call */

   if (!t_size)
      return NULL;

   /* Use bit shifts instead of division */
   i_data_blocks = (unsigned int)(t_size >> HEAP_UNIT_SHIFT);  
   if (t_size & (HEAP_UNIT_SIZE - 1))
      i_data_blocks++;  /* Pad fragment sizes to a full alignment unit block */

   i_needed_blocks = i_data_blocks + 1;  /* Add one block to hold the metadata */

   /* Loop through free list looking for first block large enough */
   for (h_link = &__sdcc_free_list; (h_block = *h_link) != NULL; h_link = &h_block->next)
   {
      if (h_block->blocks < i_needed_blocks)
         continue;

      i_remainder = h_block->blocks - i_needed_blocks;  /* Calculate remaining available space */

      if (i_remainder >= 2)
      {
         /* Split the block if the remainder can hold a header and a data unit */
         h_new_extent = (leaf *)((char *)h_block + (i_needed_blocks << HEAP_UNIT_SHIFT));  /* Advance pointer to split point */
         h_new_extent->blocks = i_remainder;  /* Assign remaining blocks to new entry */
         h_new_extent->next = h_block->next;  /* Keep list references intact */
         *h_link = h_new_extent;  /* Replace original free block entry with remainder block */
         h_block->blocks = i_needed_blocks;  /* Terminate allocated block size record */
      }
      else
      {
         *h_link = h_block->next;  /* Remainder block is too small consume the whole block */
      }

      h_block->next = __sdcc_modified_list;  /* Push block on to the modified list */
      __sdcc_modified_list = h_block;  /* Update modified list head pointer location */

      return (void *)((char *)h_block + HEAP_UNIT_SIZE);  /* Return pointer to user data */
   }
   return NULL;  /* Allocation failure, no suitable memory block found */
}

/*
 * free()
 * 
 * When blocks are freed they are returned to the free list and merged with 
 * any physically adjacent free blocks to prevent fragmentation.
 * 
 * When freeing memory this function checks that it exists on the  modified
 * list first to prevent the heap being corrupted.  The chunk is moved from 
 * the modified list and re-inserted into the free list and merged with any
 * adjacent chunks.
 * 
 */
 
void free(void *v_ptr)
{
   leaf *h_block;
   leaf *h_next_free;
   leaf *h_prev_free;
   leaf **h_modified_link;
   leaf **h_free_link;
   unsigned int i_start;

   if (!v_ptr)
      return;

   h_block = (leaf *)((char *)v_ptr - HEAP_UNIT_SIZE);  /* Recover metadata header block address */

   /* Safely isolate from modified list */
   for (h_modified_link = &__sdcc_modified_list; *h_modified_link && *h_modified_link != h_block; h_modified_link = &(*h_modified_link)->next);  /* Scan list */
   if (!*h_modified_link)
      return;  /* Safe exit guard; pointer was not tracking inside the active modified list */

   *h_modified_link = h_block->next;  /* Remove block from modified list */
   i_start = (unsigned int)h_block;

   /* Find insertion slot in address-sorted list */
   h_prev_free = NULL;
   h_free_link = &__sdcc_free_list;
   while (*h_free_link && (unsigned int)*h_free_link < i_start)
   {
      h_prev_free = *h_free_link;  /* Update trailing free block pointer tracking reference */
      h_free_link = &(*h_free_link)->next;  /* Move search pointer downward */
   }

   h_next_free = *h_free_link;  /* Assign subsequent free node identifier reference */
    
   /* Merge adjacent free blocks where possible */
   if (h_prev_free && ((unsigned int)h_prev_free + (h_prev_free->blocks << HEAP_UNIT_SHIFT) == i_start))
   {
      /* Merge into previous block */
      h_prev_free->blocks += h_block->blocks;  /* Combine blocks into previous entry record */
      if (h_next_free && ((unsigned int)h_prev_free + (h_prev_free->blocks << HEAP_UNIT_SHIFT) == (unsigned int)h_next_free))  /* Does this block fit into be gap between the last free block and the next */
      {
         h_prev_free->blocks += h_next_free->blocks;  /* Merge next block */
         h_prev_free->next = h_next_free->next;  /* Unlink next block */
      }
   }
   else
   {
      /* Cannot merge with previous block so insert block into list */
      h_block->next = h_next_free;  /* Add into free list */
      *h_free_link = h_block;
      if (h_next_free && (i_start + (h_block->blocks << HEAP_UNIT_SHIFT) == (unsigned int)h_next_free))
      {
         /* Merge next block into this one */
         h_block->blocks += h_next_free->blocks;
         h_block->next = h_next_free->next;
      }
   }
}
