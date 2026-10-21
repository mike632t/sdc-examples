/*
 * sdc-bdos.c
 *
 * Copyright(C) 2026   MT
 *
 * Read from the keyboard using the BDOS.
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
 * 18 Oct 26   0.1   - Initial version - MT
 * 
 * To Do             -
 *
 */

#define  NAME        "sdc-bdos"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#include "sdc-cpm.h" /* bdos() */
#include "stdio.h"   /* printf() */


int main(int argc, char* argv[])
{
   unsigned char buffer[82];

   buffer[0] = 80;  /* Maximum input length */
   buffer[1] = 0;

   printf("Enter Text : ");

   bdos(BDOS$Read_Str, (unsigned int)buffer);
   buffer[2 + buffer[1]] = '\0';
   putchar('\n');

   printf("Text : %s\n", &buffer[2]);
   return 0;
}

