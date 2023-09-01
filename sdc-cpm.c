/*
 * sdc-cpm.c - interface to CP/M BDOS functions.
 *
 * Copyright(C) 2023   MT
 *
 * Provides support for the CP/M operating system.
 *
 * sdcc -mz80 -c sdc-cpm.c
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
 * 20 Aug 23         - Initial version - MT
 * 29 Aug 23         - To maintain compatibility with older versions of the
 *                     compiler  this  routine can now use  either  calling
 *                     convention for parameter passing - MT
 * 
 * ToDo
 *
 */

#include "sdc-cpm.h"

/* 
 * Calling convention
 * 
 * Parameters            __SDCCCALL = 0          __SDCCCALL = 1
 * 
 *       char              stack                   A_reg
 *       int               stack                   HL_reg
 *       char, char        stack, stack+1          A_reg, L_reg
 *       char, int         stack, stack+1          A_reg, DE_reg
 *       int, int          stack, stack+2          HL_reg, DE_reg
 *       int, char         stack, stack+2          HL_reg, stack
 * 
 * Return
 * 
 *       char              L_reg                   A_reg
 *       int               HL_reg                  DE_reg
 */


#if __SDCCCALL /* Use new calling convention */

unsigned int bdos(unsigned char A_reg, unsigned int DE_reg)
{
	A_reg;
	DE_reg;
	__asm
         .z80
         ld		c,a         ; DE already points to string!
         call	#5
         ex    de,hl       ; Return result in DE
         ret
	__endasm;
	return 0;
}

#else /* Use the original calling convention */

unsigned int bdos(unsigned char c_byte, unsigned int i_word) __naked
{
	c_byte;
	i_word;
	__asm
         ld    hl,#2
         add   hl,sp
         ld		c,(hl)
         inc	hl
         ld		e,(hl)
         inc	hl
         ld		d,(hl)
         call	#5
         ld    a,h
         ret
	__endasm;
}

#endif

int putchar(int c) 
{
   if (c == '\n') bdos(C_WRITE, '\r');
   bdos(C_WRITE, c);
   return 0;
}
