/*
 * sdc-calls.c
 *
 * Copyright(C) 2023   MT
 *
 * Demonstrates how to write functions in assembler.
 * 
 * int rand()        - A random number generator (no parameter passing).
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
 * 31 Aug 23   0.1   - Initial version - MT
 * 27 Aug 26   0.2   - Replaced the standard random number generator with a
 *                     random using xorshift operations and a locally saved
 *                     seed - MT
 * 29 Sep 26   0.3   - Random  number  generator always returns a  positive 
 *                     integer - MT 
 * 
 * To Do             - 
 *
 */

#define  NAME        "sdc-call"
#define  VERSION     "0.3"
#define  BUILD       "0003"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#define  MAX_ROWS    50000

#include "stdio.h"            /* Provides printf() for printing output. */

/* 
 * rand()
 * 
 * Arguments - None
 * Returns   - Integer
 * 
 * Generates  a psudo  random  number  using  George  MARSAGLIA's  xorshift 
 * algorithm.   This  generates a 16-bit pseudo-random number  by  applying 
 * three  successive   xorshift  operations, where  the  current  value  is
 * exclusive-ORed with a shifted copy of itself.
 * 
 *    i_seed ^= i_seed << 7;
 *    i_seed ^= i_seed >> 9;
 *    i_seed ^= i_seed << 8;
 * 
 * This version was implemented in Z80 assembly by John METCALF. 
 * 
 * http://www.retroprogramming.com/2017/07/xorshift-pseudorandom-numbers-in-z80.html
 * 
 * 27 Aug 26   0.1   - Initial version - MT
 * 29 Sep 26   0.2   - Always returns a positive integer (making the result
 *                     a signed integer allows the performance of this code
 *                     to be compared to the standard rand() function) - MT
 * 
 * 
 */

unsigned int rand() __naked
{
      __asm
      ld    hl,(seed)   ; Load seed into HL.
      ;
      ld    a,h   
      rra
      ld    a,l
      rra
      xor   h
      ld    h,a
      ld    a,l
      rra
      ld    a,h
      rra
      xor   l
      ld    l,a
      xor   h
      ld    h,a         ; Result in HL.
      ld    (seed),hl   ; Save new seed.
      res   7,h         ; Clear the sign bit.
      ret
;
      .area _DATA       ; Data area.
seed: .dw   1
   __endasm;
}


int main() 
{
   unsigned int i_count;
   int i_counter;

   for( i_count = 1; i_count <= MAX_ROWS; i_count++)  
   {
      for( i_counter = 0; i_counter < 10; i_counter++)  /* Ten numbers per line */
         printf("%6d\t", rand());
      printf("\n");
   }
   return 0;
}
