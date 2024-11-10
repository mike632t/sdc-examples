
/*
 * sdc-strings.c - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Classic hello world program.
 * 
 *    sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0.rel sdc-cpm.rel sdc-strings.c 
 * 
 *    sdobjcopy -Iihex -Obinary --gap-fill 0 sdc-strings.ihx sdc-strings.com
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

#define  NAME        "sdc-strings"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date */
 
#include <stdio.h>
#include "sdc-cpm.h"

void put_s(char* s) /* Put string without newline ! */
{
    while(*s) putchar(*s++);
}

void print(int argc, const char *argv[])
{
   int i_count;
   
   for (i_count = 0; i_count < argc; i_count++)
   {
      put_s(argv[i_count]);
      putchar(' ');
   }
  putchar('\n');
}


int main()
{
   const char *strings[8];
   strings[0] = "the";
   strings[1] = "quick";
   strings[2] = "brown";
   strings[3] = "fox";
   strings[4] = "jumps";
   strings[5] = "over";
   strings[6] = "the lazy";
   strings[7] = "dog";

   print(8, strings);
   return 0;
}
