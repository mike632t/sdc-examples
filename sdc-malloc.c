/*
 * sdc-malloc.c
 *
 * Copyright(C) 2026   MT
 *
 * Uses  malloc() to allocate memory from the heap.  Since the heap size is
 * fixed you can experiment with the block size to see how this affects the
 * number of blocks that can be allocated.
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

#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SIZE 32

int main(void)
{
   void *p_block;  /* Pointer to current block */ 
   void *p_last = NULL;  /* Saves pointer to the last block */
   void *p_previous;  /* Stores the pointer to previous block (when using free()) */

   int i_size = INITIAL_SIZE;
   int i_stop = 1;
   int i_counter = 0;
   int i_count = 0;
   long i_total = 0;

   if (i_size < sizeof(void *)) i_size = sizeof(void *);
   
   for (i_counter = 0; i_counter < 12; ++i_counter)
   {
      i_count = 0;
      i_total = 0;
      p_last = NULL;  /* Reset linked list for this test */

      while ((p_block = malloc(i_size)) != NULL) 
      {
         /* Store previous block address in this block */
         *(void **)p_block = p_last; /* Link this block to the previous block */
         p_last = p_block;  /* Save pointer to this block  */

         i_count++;
         i_total += i_size;
      }

      if (i_count > 0)
      {
         if (i_counter > 0) printf("\n");
         printf("Block size: %d bytes\n", i_size);
         printf("Allocated:  %ld bytes\n", i_total);
         printf("Allocated:  %d blocks\n", i_count);

         i_count = 0;
         p_block = p_last;
         while (p_block != NULL) 
         {
            p_previous = *(void **)p_block;  /* Get the next block before freeing  the current block*/
            free(p_block);  /* Free the current block */
            p_block = p_previous;  /* Move to the previous block */
            i_count++;
         }
         printf("Freed:      %d blocks\n", i_count);
         i_size <<= 1;
      }
   }
   return 0;
}
