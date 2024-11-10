/*
 * sdc-julia.c - Julia Set.
 *
 * Copyright(C) 2023 - MT
 *
 * Displays an an ASCII representation of a Julia set.
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
 * https://en.wikipedia.org/wiki/Julia_set
 *
 * 22 Sep 23   0.1   - Initial version - MT
 *
 * To Do             -
 *
 */

#include <stdio.h>     /* fprintf(), etc */

#define NAME           "sdc-julia"
#define VERSION        "0.8"
#define BUILD          "0092"
#define DATE           "29 Jan 22"
#define AUTHOR         "MT"

#if defined(DEBUG)
#define debug(code) do {fprintf(stderr, "Debug\t: %s line : %d : ", \
            __FILE__, __LINE__); code;} while(0)
#else
#define debug(code)
#endif

void main()
{
/*
   const char* c_ascii[] =
      {"\x1b[0m ",   "\x1b[0;41m.", "\x1b[0;42m:", "\x1b[0;43m-", "\x1b[0;44m=",
      "\x1b[0;45m+", "\x1b[0;46m*", "\x1b[0;47m#", "\x1b[0;47m%", "\x1b[0;47m$",
      "\x1b[0;47m@"};
 */
   const char* c_ascii[] = {" ", ".", ":", "-", "=", "+", "*", "#", "%", "$", "@"};
   const float xa = -1.5;  /* Left edge      */
   const float xb = 1.5;   /* Right edge     */
   const float ya = -1.0;  /* Top edge       */
   const float yb = 1.0;   /* Bottom edge    */
   const float xd = 0.025; /* X step size    */
   const float yd = 0.05;  /* Y step size    */
   const int m = 100;      /* Max iterations */

   float x, y, zr, zr1, zi;
   float cr = -.79;        /* -.75, -.74543 */
   float ci = 0.15;        /* 0.11, 0.11301 */
   float r = 2.0;
   int i;

   for (y = ya; y < yb; y = y + yd)
   {
      for (x = xa; x < xb; x = x + xd)
      {
         zr = x;
         zi = y;
         i = 0;
         while ((zr *zr + zi * zi < r*r) && i < m)
         {
            zr1 = zr*zr - zi*zi;
            zi = 2 * zr * zi + ci;
            zr = zr1 + cr;
            i++;
         }
         printf("%s", c_ascii[i/10]);
      }
      printf("%s\n",  c_ascii[0]);
   }
}
