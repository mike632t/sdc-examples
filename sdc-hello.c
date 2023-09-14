/*
 * sdc-hello.c - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Classic hello world program.
 * 
 *    sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0.rel sdc-cpm.rel sdc-hello.c 
 * 
 *    sdobjcopy -Iihex -Obinary --gap-fill 0 sdc-hello.ihx sdc-hello.com
 * 
 * With sdcc 4.0.0 the code is 398 bytes long and takes 64,155 clock cycles
 * to execute aproximatly 16 ms (at 4 Mhz), upgrading to sdcc 4.2.0 reduces
 * this to 363 bytes and 34,535 clock cycles which is approximatly 8 ms.
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
 * 
 * ToDo
 *
 */

#define  NAME        "sdc-hello"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date */
 
#include <stdio.h>

void main()
{
   int i_count;
   
   for (i_count = 0; i_count <= 9; i_count++)
   {
      puts("Hello World !!");
   }
}
