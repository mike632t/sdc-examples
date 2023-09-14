/*
 * sdc-mandlebrot.c - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Displays an ASCII mandlebrot set.
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
 * 03 Sep 23         - Initial version - MT
 * 
 * ToDo              - Get month/year from clock or command line - MT
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
   const char* a[8] = {"\x1b[0m", "\x1b[0;41m", "\x1b[0;42m", "\x1b[0;43m", 
      "\x1b[0;44m", "\x1b[0;45m", "\x1b[0;46m", "\x1b[0;47m"};
   
   long x, y; /* Don't know why but this code only works with sdcc if x and y are long */
   int x2, y2, x0, y0, i, n;
   char c;

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
         c = ' ';
         n = 0;
         i = 0;
         while (i < m)
         {
            x2 = (x * x) / 200;
            y2 = (y * y) / 200;
            if (x2 + y2 >= 800)
            {
               if (i > 9) c = '0'; else c = '0' + i;
               if (i > 7) n = 7; else n = i;
               i = m;
            }
            y = (x * y / 100 + y0);
            x = (x2 - y2 + x0);
            i++;
         }
         printf("%s%c", a[n], c);
         x0 = x0 + xd;
      }
      printf("%s\n", a[0]);
      y0 = y0 - yd;
   }
}
