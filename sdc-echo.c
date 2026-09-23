/*
 * echo.c
 *
 * Copyright(C) 2026 - MT
 *
 * Echos the command line on the console.
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
 * with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *
 * 12 Sep 26   0.1   - Initial version - MT
 *
 */

#define  NAME        "sdc-echo"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#include "stdio.h"  /* Provides printf() for printing output */

int main(int argc, char* argv[])
{
   int i_count;

   /* Print each argument, adding a newline after the last one */
   for (i_count = 0; i_count < argc; i_count++)  
      printf("'%s'", argv[i_count]);
   if (--argc > 0) printf ("\n");
   return 0;
}

