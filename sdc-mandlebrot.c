/*
 * mandlebrot.c
 *
 * Copyright(C) 2023   MT
 *
 * Displays an ASCII Mandelbrot set.
 * 
 * For sdcc compile using:
 *    sdasz80 -o sdc-crt0.s
 *    sdcc -mz80 -c sdc-cpm.c
 *    sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0.rel sdc-cpm.rel sdc-mandlebrot.c 
 *    sdobjcopy -Iihex -Obinary --gap-fill 0 sdc-mandlebrot.ihx sdc-mandlebrot.com
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
 * 03 Sep 23   0.1   - Initial version - MT
 * 
 * ToDo              -
 *
 */

#define  NAME        "sdc-mandlebrot"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date */

#include <stdio.h>

void main()
{
   const char* c_ascii[] = {" ", ".", ":", "-", "=", "+", "*", "#", "%", "%", "%", "$", "$", "$", "$", "@", "@", "@", "@", "@"};

   long x, y;
   int x2, y2, x0, y0, i, n;

   int xa = -500;
   int xb = 300;
   int ya = 240;
   int yb = -250;
   int xd = 7;
   int yd= 15;
   int m = 20;
   
   y0 = ya;
   while (y0 > yb)
   {
      x0 = xa;
      while (x0 < xb)
      {
         y = 0;
         x = 0;
         n = 0;
         i = 0;
         while (i < m)
         {
            x2 = (x * x) / 200;
            y2 = (y * y) / 200;
            if ((x2 + y2) >= 800)
            {
               //printf("%d ", i);
               if (i > 19) n = 0; else n = i;
               i = m;
            }
            y = (x * y / 100 + y0);
            x = (x2 - y2 + x0);
            i++;
         }
         printf("%s", c_ascii[n]);
         x0 = x0 + xd;
      }
      printf("\n");
      y0 = y0 - yd;
   }
}
