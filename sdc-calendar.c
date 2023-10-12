/*
 * sdc-calendar.c - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Displays the calendar for a month.
 *
 * With sdcc 4.0.0 the code is 5168 bytes long, upgrading to sdcc 4.2.0
 * reduces this to 4389 bytes.
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
 * 03 Sep 23         - Initial version - MT
 * 12 Sep 23         - Gets month/year from command line - MT
 *
 */

#define  NAME        "sdc-hello"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date */

#include <stdio.h>
#include <stdlib.h>

int i_isLeapYear(int i_year)
/*
 * Returns true if the year is a leap year for years > 1752.
 *
 */
{
   return(i_year % 4 == 0 && i_year % 100 != 0 || i_year % 400 == 0);
}

int i_weekday(int i_day, int i_month, int i_year)
/*
 * Returns the day of the week (Sun = 0, Mon = 1, etc) for a given date
 * for 1 <= i_month <= 12,  i_year > 1752.
 *
 * https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Sakamoto's_methods
 *
 */
{
   int i_lookup[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; /* Don't use static (sdcc bug?) */
   if (i_month < 3 )
   {
      i_year -= 1;
   }
   return (i_year + i_year / 4 - i_year / 100 + i_year / 400 + i_lookup[i_month - 1] + i_day) % 7;
}

void v_print_calendar(int i_month, int i_year)
{
   const char* s_month[] = {  "    January",  "   February",   "     March",  "     April",
                              "      May",    "     June",     "     July",   "    August",
                              "   September", "    October",   "   November", "   December" };

   int i_length[] = {31, 28 + i_isLeapYear(i_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; /* Doesn't have to be declaired before any statements !! */
   int i_start = i_weekday(1, i_month, i_year);

   printf("%s %4d\n",s_month[i_month - 1], i_year); /* Print heading (month and year) and days of the week. */
   printf("Su Mo Tu We Th Fr Sa\n");

   for (int i_count = 0; i_count < i_start; i_count++) /* Indent the first line if necessary */
   {
      printf("   ");
   }

   for (int i_day = 1; i_day <= i_length[i_month - 1]; i_day++)
   {
      printf("%2d ", i_day);
      if (++i_start > 6) /* End of week, print a newline.  */
      {
         i_start = 0;
         printf("\n");
      }
   }
   if (i_start) /* print a new line after the last day unless it is the end of a week. */
   {
      printf("\n");
   }
}

int main(int argc, char* argv[])
{
   if (argc == 3)
   {
      int i_month = atoi(argv[1]);
      int i_year = atoi(argv[2]);

      if (i_month > 0 && i_month < 13 && i_year > 1752)
         v_print_calendar(i_month, i_year);
      else
         printf("Year or month out of range.\n");
   }
   else
      printf("Usage: cal month year\n");
   return 0;
}
