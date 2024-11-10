/*
 * sdc-cpm.c
 *
 * Copyright(C) 2023   MT
 *
 * Provides minimal support for the CP/M operating system.
 *
 * For sdcc compile using:
 *    sdcc -mz80 -c sdc-cpm.c
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
 * 20 Aug 23   0.1   - Initial version - MT
 * 
 * ToDo              -
 *
 */

#include "sdc-cpm.h"

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


int putchar(int c) 
{
   if (c == '\n') bdos(C_WRITE, '\r');
   bdos(C_WRITE, c);
   return 0;
}
