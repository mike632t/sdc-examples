/*
 * sdc-version.c - Example program for SDCC.
 *
 * Copyright(C) 2022   MT
 *
 * A classic 'Hello World' program used to demonstrate how to cross compile
 * code for a Z80 CPM target using SDCC.
 *
 *    sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0.rel sdc-version.c
 *
 *    sdobjcopy -Iihex -Obinary --gap-fill 0 sdc-version.ihx sdc-version.com
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
 * ToDo
 *
 */

#define  NAME        "sdc-version"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date */

#include <stdio.h>
#include <stdlib.h>

#define  false       0
#define  true        !false

void v_version() /* Display version information */
{
   printf("%s: Version %s\n", NAME, VERSION);
   printf("Copyright(C) %s %s\n", COPYRIGHT, AUTHOR);
   printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
   printf("This is free software: you are free to change and redistribute it.\n");
   printf("There is NO WARRANTY, to the extent permitted by law.\n");
}

void main()
{
   v_version();
}


