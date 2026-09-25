/*
 * hello.c
 *
 * Copyright(C) 2023   MT
 *
 * The obligatory hello world program.
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
 * 24 Nov 24         - Removed unnecessary (and non standard) include) - MT
 *
 */

#define  NAME        "sdc-hello"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7)  /* Extract copyright year from date. */
 
#include <stdio.h>

void main()
{
   int i_count;
   
   for (i_count = 0; i_count <= 9; i_count++)
   {
      puts("Hello World !!");
   }
}
