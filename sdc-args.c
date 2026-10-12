/*
 * sdc-echo.c
 *
 * Copyright(C) 2023   MT
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
 * 22 Nov 24   0.1   - Initial version - MT
 * 12 Sep 26   0.2   - Use printf() -MT
 * 
 * To Do             -
 *
 */

#define  NAME        "sdc-args"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#include "stdio.h"  /* Provides printf() for printing output */

int main(int argc, char* argv[])
{
   int i_count;
   unsigned char * h_buffer;

   /* Print each argument, adding a newline after the last one */
   for (i_count = 0; i_count < argc; i_count++)  
      printf("0x%04x  argv[%0d] = '%s'\n", argv[i_count], i_count, argv[i_count]);
   printf("\n");

   /* Display contents of CP/M command line buffer */
   h_buffer = (unsigned char *)0x0080;
   for (i_count = 0; i_count < 32; i_count++) {
      if (i_count % 16 == 0) printf("\n0x%04x  ", (unsigned int)&h_buffer[i_count]);
      printf("0x%02x '%c' ", h_buffer[i_count],
      (h_buffer[i_count] >= 32 && h_buffer[i_count] <= 126) ? h_buffer[i_count] : '.');
   }
   printf("\n");

   /* Display contents of argv[] */
   h_buffer = (unsigned int *)0x0103;
   for (i_count = 0; i_count < 8; i_count++) {
       if (i_count % 16 == 0) printf("\n0x%04x  ", (unsigned int)&h_buffer[i_count * 2]);
       printf("0x%04x   ", h_buffer[i_count * 2]);
   }
   printf("\n");

   return 0;
}

