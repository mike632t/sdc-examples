/*
 * sdc-compare.c - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Compares several pairs of values and checks the results. Originally used
 * to demonstrate the following issue with the MESSC compiler:
 * 
 * https://github.com/MiguelVis/mescc/issues/2
 * 
 * The results demonstrate that SDCC does NOT have the same problem!
 *
 * sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0-args.rel sdc-compare.c
 *
 * sdobjcopy -Iihex -Obinary --gap-fill 0 sdc-compare.ihx sdc-compare.com
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
 * 06 Jan 21   0.1   - Initial version - MT
 *
 * To Do:            - Figure out why this doesn't work!
 */

#define  NAME        "sdc-hello"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date */

#include <stdio.h>

/** #define DEBUG */

#ifndef debug /* Don't redefine macro if already defined. */
#if defined(DEBUG)
#define debug(code) do {printf("Debug\t: %s line : %d : ", \
            __FILE__, __LINE__); code;} while(0)
#else
#define debug(code)
#endif
#endif

int min(int m, int n)
{
   if (m < n)
   {
      debug(printf("min %12d, %d = %d (a)\n", n, m, m));
      return m;
   }
   else
   {
      debug(printf ("min %12d, %d = %d (b)\n", m, n, n));
      return n;
   }
}

int max(int m, int n)
{
   if (m > n)
   {
      debug(printf ("max %12d, %d = %d (c)\n", n, m, m));
      return m;
   }
   else
   {
      debug(printf ("max %12d, %d = %d (d)\n", m, n, n));
      return n;
   }
}

void main()
{
   int a[15];
   int b[15];
   int i;

   a[1] = 0;      b[1] = 32767;
   a[2] = 0;      b[2] = 16383;
   a[3] = -16384; b[3] = 0;
   a[4] = -32678; b[4] = 0;

   a[5] =  16384; b[5] = 32767;
   a[6] = -32768; b[6] = -16384;
   a[7] = -1;     b[7] = 32767;
   a[8] = -32767; b[8] = 1;
   a[9] = -2;     b[9] = 32767;
   a[10] = -32767;b[10] = 2;
   a[11] = -32768;b[11] = 32767;
   a[12] = -16384;b[12] = 16384;
   a[13] = -8192; b[13] = 24576;
   a[14] =-16384; b[14] = 16384;

   for ( i = 1; i <= 14; i++)
   {
      if (a[i] == min(a[i], b[i]))
         printf ("%3d min(%6d, %-6d) = %-6d \tPassed\n",
            i, a[i], b[i], a[i]);
      else
         printf ("%3d min(%6d, %-6d) = %-6d \tFailed\n",
            i, a[i], b[i], b[i]);
   }

   printf("\n");

   for ( i = 1; i <= 14; i++)
   {
      if (b[i] == max(a[i], b[i]))
         printf ("%3d max(%6d, %-6d) = %-6d \tPassed\n",
            i, a[i], b[i], b[i]);
      else
         printf ("%3d max(%6d, %-6d) = %-6d \tFailed\n",
            i, a[i], b[i], a[i]);
   }
}
