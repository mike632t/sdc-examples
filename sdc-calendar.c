/*
 * calendar.c
 *
 * Copyright(C) 2023   MT
 *
 * Displays the calendar for a month or whole year.
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
 * 24 Nov 24   0.1   - Initial version - MT
 * 16 Sep 26   0.2   - Can display a whole year or a single month - MT
 * 
 * ToDo              - Improve error checking in argument parser
 *                   - Add options for '--help', '--version'
 *                   - Allow the user to specify the number of months to be
 *                     shown  (print_months() can already do this) and  the 
 *                     number of months to include in a row
 *                   - Display the year before the rest of the calendar
 *                     
 *
 */

#define  NAME        "sdc-calendar"
#define  VERSION     "0.2"
#define  BUILD       "0002"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7)  /* Extract copyright year from date */
 
#include <stdio.h>
#include <stdlib.h>

int i_isLeapYear(int i_year)  /* Returns true if the year is a leap year for years > 1752 */
{
   return(i_year % 4 == 0 && i_year % 100 != 0 || i_year % 400 == 0);
}

int i_weekday(int i_day, int i_month, int i_year)  /* Returns the day of the week (Sun = 0, Mon = 1, etc) */

/* https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week#Sakamoto's_methods */
{
   int i_lookup[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4}; /* Don't use static (sdcc bug?) */
   if (i_month < 3 ) 
   {
      i_year -= 1;
   }
   return (i_year + i_year / 4 - i_year / 100 + i_year / 400 + i_lookup[i_month - 1] + i_day) % 7;
}

void v_print_months(int i_start, int i_months, int i_year)  /* Display multiple months starting from the specified month/year */
{
   const char* s_month[] = {"    January", "   February", "     March", "     April", 
                            "      May", "     June", "     July", "    August", 
                            "   September", "    October", "   November", "   December" };
   
   int i_length[] = {31, 28 + i_isLeapYear(i_year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  /* Doesn't have to be declared before any statements !! */
   int i_day;
   int i_week;
   int i_month;
   int i_current;  /* Current year */
   int i_first;  /* First day of week */
   int i_count;
   int i_counter;
   int i_row;  /* Number of months in a row */

   /* It took me a while to figure out how to print multiple months (trying 
    * to reuse the original code won't work).  I finally worked it out when 
    * realized that I needed to do wasn't to display a month at a time, but
    * display all the headers for each month, then all the days of the week
    * followed by the days for each week */
    
   while (i_months > 0)
   {
      if (i_months > 3) i_row = 3; else i_row = i_months;  /* Number of months to display on a row (max 3) */

      for (i_count = 0; i_count < i_row; i_count++)  /* Print heading for each month */
      {
         /* Calculate the current month and year */
         i_month = i_start + i_count;
         i_current = i_year + (i_month - 1) / 12;
         i_month = (i_month - 1) % 12 + 1;

         printf("%11s %4d    ",s_month[i_month - 1], i_current);  /* Print heading (month and year) */
         if (i_count < i_row - 1) printf("  "); else printf("\n");  /* Add some spaces between months or a newline */
      }
      
      for (i_count = 0; i_count < i_row; i_count++)  /* Print days of the week for each month */
      {
         printf("Su Mo Tu We Th Fr Sa");
         if (i_count < i_row - 1) printf("  "); else printf("\n");  /* Add some spaces between months or a newline */
      }
      
      for (i_week = 0; i_week < 6; i_week++)  /* Print each of the six possible weeks for each month */
      {
         for (i_count = 0; i_count < i_row; i_count++)  /* Print the current week */
         {
            /* Calculate the current month and year */
            i_month = i_start + i_count;
            i_current = i_year + (i_month - 1) / 12;
            i_month = (i_month - 1) % 12 + 1;

            i_length[1] = 28 + i_isLeapYear(i_current);
            i_first = i_weekday(1, i_month, i_current);

            for (i_counter = 0; i_counter < 7; i_counter++)  /* Print the dates for each day of the current week */
            {
               i_day = i_week * 7 + i_counter - i_first + 1;
               if (i_day >= 1 && i_day <= i_length[i_month - 1]) printf("%2d ", i_day); else printf("   ");
            }
            if (i_count < i_row - 1) printf(" "); else printf("\n");  /* Add some spaces between months or a newline */
         }
      }
      
      printf("\n");  /* Print a blank line between rows */

      i_start += 3;   /* Find start of the next row */
      i_months -= i_row;  /* Reduce the number of months left to display by the number of months in a row */
   }
}


int main(int argc, char* argv[])
{
   int i_month;
   int i_year;

   if (argc == 2)
   {
      i_year = atoi(argv[1]);

      if (i_year > 1752)  /* Check year is valid */
      {
         v_print_months(1, 12, i_year);  /* Display all 12 months in the year */
      }
      else
         printf("Year out of range.\n");
   }
   else if (argc == 3)
   {
      i_month = atoi(argv[1]);
      i_year = atoi(argv[2]);

      if (i_month > 0 && i_month < 13 && i_year > 1752)  /* Check month and year are valid */
      {
         v_print_months(i_month, 1, i_year);  /* Just display a single month */
      }
      else
         printf("Year or month out of range.\n");
   }
   else
      printf("Usage: cal [month] year\n");
   return 0;
}
