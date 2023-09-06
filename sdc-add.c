/*
 * sdc-add.c  - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Demonstrates how to call some of the common math functions using SDCC.
 *
 * sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0.rel sdc-cpm.rel sdc-add.c
 *
 * sdobjcopy -Iihex -Obinary --gap-fill 0 sdc-add.ihx sdc-add.com
 * 
 * With sdcc 4.0.0 this code is 3764 bytes long and takes aproximatly 69 ms
 * to execute (at 4Mhz), upgrading to sdcc 4.2.0 reduces this to 3265 bytes
 * 49 ms!
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
 * 31 Aug 23         - Initial version - MT
 *
 */

#define VERSION        "0.1"
#define BUILD          "0001"
#define DATE           "20 Aug 23"
#define AUTHOR         "MT"

#include <stdio.h>
#include "sdc-cpm.h"

void putint(int i_number)
{
   if (i_number < 0) 
   {
      i_number *= -1;
      putchar('-');
   }
   if (i_number > 9) 
   {
      putint(i_number / 10);
   }
   putchar('0'+ (i_number % 10));
}

void puti(int i_number)
{
   putint(i_number);
   putchar('\n');
}

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
int Add_Int(int i_inta, int i_intb) __naked
{
   i_inta; /* Avoids 'unreferenced function argument' warning */
   i_intb;
   
   __asm
   add hl,de
   ex  de,hl
   ret
   __endasm;
}

#else

int Add_Int(int i_inta, int i_intb) __naked
{
   i_inta; /* Avoids 'unreferenced function argument' warning */
   i_intb;
   
   __asm
   ld iy,#2
   add iy,sp

   ld e,(iy)   ; Load i_inta into DE (low / high)
   ld d,1(iy)  ;

   ld l,2(iy)  ; Load i_intb into HL.
   ld h,3(iy)

   add hl,de   ; result in HL.

   ret
   __endasm;
}

#endif

void main()
{
   int i_result = 1024;
   puti(Add_Int(612, i_result));
}
