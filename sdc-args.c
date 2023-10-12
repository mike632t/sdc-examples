/*
 * sdc-args.c - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Displays all the comamnd line arguments to demonstrate how to access the
 * command line when using SDCC to target CPM.
 * 
 * sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0-args.rel sdc-args.c 
 * 
 * sdobjcopy -Iihex -Obinary sdc-args.ihx sdc-args.com
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
 * 12 Sep 23         - Moved CP/M specific function to C runtime - MT
 * 
 * ToDo
 *
 */

#define  NAME        "sdc-args"
#define  VERSION     "0.1"
#define  BUILD       "0002"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date */
 
#include <stdio.h>

int main(int argc, char* argv[])
{
   for (int i_count = 0; i_count < argc; i_count++)
   {
      printf("'%s'\n",argv[i_count]);
   }
   return 0;
}
