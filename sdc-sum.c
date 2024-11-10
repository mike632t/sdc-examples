/*
 * sdc-sum.c  - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Demonstrates how to call some of the common math functions using SDCC.
 * 
 * sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0.rel sdc-sum.c 
 * 
 * sdobjcopy -Iihex -Obinary --gap-fill 0 sdc-sum.ihx sdc-sum.com
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
 */
 
#define VERSION        "0.1"
#define BUILD          "0001"
#define DATE           "20 Aug 23"
#define AUTHOR         "MT"

#include <math.h>

void main()
{
   float i,sum;
   sum = 0;
   for(i = 1; i <= 1000; i++)
   {
      sum+=powf(expf(sinf(atanf(i))),1/3.0); /* Note - Non standard function names */
   }
}
