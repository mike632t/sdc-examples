/*
 * sdc-getchar.c
 *
 * Copyright(C) 2023   MT
 *
 * Shows how to use getchar() to read from the keyboard.
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
 * 08 Oct 26   0.1   - Initial version - MT
 * 
 * To Do             -
 *
 */

#define  NAME        "sdc-getchar"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#include "stdio.h"  /* printf() */
#include "ctype.h"  /* isprint() */

int main(int argc, char* argv[])
{
   int c_char;

   while ((c_char = getchar()) != EOF)
      if (isprint(c_char)) putchar(c_char); else putchar('.');;

   printf("\n");

   return 0;
}

