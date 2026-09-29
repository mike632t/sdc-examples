/*
 * sdc-static.c
 *
 * Copyright(C) 2023   MT
 *
 * Test static variables can be created.
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
 * with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *
 * 22 Sep 26   0.1   - Initial version - MT
 * 29 Sep 26   0.2   - Allocate all available memory from the heap and fill
 *                     it with known values (demonstrates that the heap and
 *                     static/global variables are stored separately) - MT
 * 
 * To Do             -
 *
 */
 
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* malloc() */
#include <string.h>    /* memset() */

#define  SIZE     64
#define  COUNT    256

static unsigned long l_value = 0x12345678UL;
long l_counter;

void main(void)
{
   unsigned char *h_value = (unsigned char *) &l_value;
   unsigned char *h_buffer;
   
   l_counter = l_value;

   printf("\t0x12345678 = `%010lu`\n", l_counter);
   printf("\t0x12345678 = `0x%08lx`\n", l_counter);
   printf("\t0x12345678 = `%010lu`\n", l_value);
   printf("\t0x12345678 = `0x%08lx`\n", l_value);
   printf("\t0x12345678 = `0x%02x%02x%02x%02x`\n", h_value[3], h_value[2], h_value[1], h_value[0]); /* Order reversed (little endian) */ 

   for (int i_count = 1; i_count < COUNT; i_count++)
   {
      h_buffer = (unsigned char *) malloc(SIZE);
      if (h_buffer == NULL) 
      {
         printf("\nError: Heap allocation failed!\n\n"); 
         break;
      }
      else 
      {
         memset(h_buffer, 0x55, SIZE);
         printf("%4d\t0x%04x, 0x%02x .. %02x\n", i_count, h_buffer, h_buffer[0], h_buffer[SIZE -1]);
      }
   }
   
   printf("\t0x12345678 = `%010lu`\n", l_counter);
   printf("\t0x12345678 = `0x%08lx`\n", l_counter);
   printf("\t0x12345678 = `%010lu`\n", l_value);
   printf("\t0x12345678 = `0x%08lx`\n", l_value);
   printf("\t0x12345678 = `0x%02x%02x%02x%02x`\n", h_value[3], h_value[2], h_value[1], h_value[0]); /* Order reversed (little endian) */ 
}
