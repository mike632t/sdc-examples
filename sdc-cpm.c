/*
 * sdc-cpm.c
 *
 * Copyright(C) 2023   MT
 *
 * Provides minimal support for the CP/M operating system.
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
 * 29 Sep 26   0.2   - Added support for both old and new calling standards 
 *                     allowing code to be built without modification using
 *                     either version - MT
 * 
 * 
 * ToDo              - Check BDOS return value.
 *
 */

#define  NAME        "sdc-cpm"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#include "sdc-cpm.h"

unsigned int bdos(unsigned char c_byte, unsigned int i_word) __naked
{
	c_byte;
	i_word;
	__asm
#if __SDCCCALL == 1
         ld    c,l         ; Move byte into C. DE already contains word.
#else
         ld    hl,#2       ; Offset to skip over the return address.
         add   hl,sp       ; Address of first arg in HL.
         ld		c,(hl)      ; Load byte into C.
         inc	hl          ; Address of second arg in HL.
         ld		e,(hl)      ; Get lo-byte of word.
         inc	hl
         ld		d,(hl)      ; Get hi-byte of word.
#endif
         call	#5          ; Call BDOS
         ld    a,h         ; Return result in a
         ret
	__endasm;
}

int putchar(int c) 
{
   if (c == '\n') bdos(C_WRITE, '\r');
   bdos(C_WRITE, c);
   return 0;
}
