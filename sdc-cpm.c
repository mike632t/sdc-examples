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

#define  CPM$BDOS    0x0005         ;

#if __SDCCCALL == 1

unsigned int bdos(unsigned char c_byte, unsigned int i_word) __naked
{
   c_byte;
   i_word;
   __asm
         ld    c,a            ; Move byte in A to C.
         jp    CPM$BDOS       ; Character already in E.
   __endasm;
}

#else

unsigned int bdos(unsigned char c_byte, unsigned int i_word) __naked
{
   c_byte;
   i_word;
   __asm
         ld    hl,#2          ; Offset into stack.
         add   hl,sp          ; Get address of byte.
         ld    c,(hl)         ; Save BDOS function in C.
         inc   hl             ; Increment pointer.
         ld    e,(hl)         ; Save word in DE (low byte hi byte).
         inc   hl
         ld    d,(hl)
         jp    CPM$BDOS       ; Return from BDOS.
   __endasm;
}

#endif

int putchar(int c)
{
   if (c == '\n') bdos(C_WRITE, '\r');
   bdos(C_WRITE, c);
   return 0;
}
