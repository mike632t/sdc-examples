/*
 * sdc-echo.c - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Reads  characters  from the console input and echos then to the  console 
 * output. 
 * 
 * sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0-args.rel sdc-cpm.rel sdc-echo.c 
 * 
 * sdobjcopy -Iihex -Obinary --gap-fill 0 sdc-echo.ihx sdc-echo.com
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
 * 16 Sep 23   0.1   - Initial version - MT
 *
 */

#define  NAME        "sdc-echo"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date */

#ifndef debug 
#if defined(DEBUG)
#define debug(code) do {printf("Debug\t: %s line : %d : ", \
            __FILE__, __LINE__); code;} while(0)
#else
#define debug(code)
#endif
#endif

#include <stdio.h>
#include <ctype.h>

int main(void)
{
   char c_char;

   while((c_char = getchar()) != 0x1a) 
   {
      if (isprint(c_char)) putchar(c_char);
      if ((c_char == 0x7f) || (c_char == 0x08)) putchar(0x08);
      if (c_char == 0x0d) putchar(0x0a);
   }
   return 0;
}
