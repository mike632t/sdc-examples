/*
 * sdc-cpm.c - interface to CP/M BDOS functions.
 *
 * Copyright(C) 2023   MT
 *
 * Provides support for the CP/M operating system.
 *
 * sdcc -mz80 -c sdc-cpm.c
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
 * 29 Aug 23         - To maintain compatibility with older versions of the
 *                     compiler  this  routine can now use  either  calling
 *                     convention for parameter passing - MT
 * 
 * ToDo
 *
 */

int putchar(int c) 
{
   if (c == '\n') bdos(C_WRITE, '\r');
   bdos(C_WRITE, c);
   return 0;
}
