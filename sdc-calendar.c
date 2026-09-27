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
 * 27 Sep 26         - Only prints blank lines between rows - MT
 *             0.3   - Checks arguments are numeric - MT
 *                   - Added error() usage() and version() functions - MT
 *                   - Added command line options - MT
 *                   - Allow the user to specify the number of months to be
 *                     shown (max 3 years) - MT
 * 
 * To Do             - Display the year before the rest of the calendar.
 *                   - Highlight current date.
 *
 */

#define  NAME          "sdc-calendar"
#define  VERSION       "0.3"
#define  BUILD         "0007"
#define  AUTHOR        "MT"
#define  COPYRIGHT     (__DATE__ + 7)  /* Extract copyright year from date */


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>

#define  MAX_YEARS     3

#define  EXIT_FAILURE  -1
#define  EXIT_SUCCESS  0

#ifndef __min
#define __min(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef __max
#define __max(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef true
#define true           1
#endif
#ifndef false
#define false          0
#endif

void v_version()  /* Display version information */
{ 
   printf("%s: Version %s.%s %s", NAME, VERSION, BUILD, COMMIT_ID);
#if defined(__compiler__)
   if (strlen(__compiler__)) printf(" %s", __compiler__);  /* Include compiler version if defined - it could be defined as a null string */
#endif
   if (__DATE__[4] == ' ') printf(" 0"); else printf(" %c", __DATE__[4]);
   printf("%c %c%c%c %s %s\n", __DATE__[5],
      __DATE__[0], __DATE__[1], __DATE__[2], &__DATE__[9], __TIME__ );
   printf("Copyright(C) %s %s\n", __DATE__ +7, AUTHOR);
   printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
   printf("This is free software: you are free to change and redistribute it.\n");
   printf("There is NO WARRANTY, to the extent permitted by law.\n");
}
 
void v_usage()  /* Display help text */
{ 
   printf("Usage: %s [MONTH] YEAR [OPTION]...\n", NAME);
   printf("Display the calendar for a month or year.\n\n");
   printf("  /?, /help                display this help and exit\n");
   printf("  /c, /count nn            number of months to show (max 3 years)\n");
   printf("      /version             output version information and exit\n\n");
}

int i_error(int i_errno, const char *s_format, ...)  /* Print formatted error message and exit returning errno */
{
   va_list t_args;
   if (!(i_errno)) i_errno = -1;  /* If errno not set return -1 */
   va_start(t_args, s_format);
   printf("%s: ", NAME);
   vprintf(s_format, t_args);
   va_end(t_args);
   /** exit(i_errno); */
   return i_errno;
}

int isnumeric(const char *s_text) 
{
int i_count = 0;

while (s_text[i_count] != '\0') 
{
   if (s_text[i_count] < '0' || s_text[i_count] > '9') 
      return(false);  /* Return false upon finding a non-digit */
   i_count ++;
}
return(true); 
}

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
      i_start += 3;   /* Find start of the next row */
      i_months -= i_row;  /* Reduce the number of months left to display by the number of months in a row */ 
      if (i_months > 0) printf ("\n");  /* Only print a blank line if there are more rows left */
   }
}


int main(int argc, char* argv[])
{
   int i_months = 1;
   int i_month = 0;
   int i_year = 0;

   int i_count, i_index, i_offset;
   int i_status = EXIT_SUCCESS;

   /* Parse command line options without using 'getopt' or 'argeparse' */
   for (i_count = 1; i_count < argc; i_count++) 
   {
      if (argv[i_count][0] == '/') 
      {
         i_index = 1;
         while (argv[i_count][i_index] != 0 && !i_status) 
         {
            i_index = __max(strlen(argv[i_count]), 4);  /* Need at least 4 characters */
            if (!strncmp(argv[i_count], "/?", strlen(argv[i_count])) || (!strncmp(argv[i_count], "/HELP", i_index))) 
            {
               v_usage();
               return(0);
            }
            else if (!strncmp(argv[i_count], "/VERSION", i_index)) 
            {
               v_version();
               return(0);
            }
            else if (!strncmp(argv[i_count], "/C", i_index) || !strncmp(argv[i_count], "/COUNT", i_index))
            {
               if (i_count + 1 < argc)
               {
                  if (isnumeric(argv[i_count + 1]))
                  {
                     i_months = atoi(argv[i_count + 1]);
                     if (i_months < 12 * MAX_YEARS +1)
                     {
                        if (i_count + 2 < argc)  /* Remove the parameter from the arguments */
                           for (i_offset = i_count + 1; i_offset < argc - 1; i_offset++)
                              argv[i_offset] = argv[i_offset + 1];
                        argc--;
                     }
                     else 
                        i_status = i_error(EXIT_FAILURE, "Out of range\n");
                  }
                  else
                     i_status = i_error(EXIT_FAILURE, "Invalid argument -- '%s'\n", argv[i_count]);
               }
               else
                  i_status = i_error(EXIT_FAILURE, "Option requires an argument -- '%s'\n", argv[i_count]);
            }
            else 
            {
               i_status = i_error(EXIT_FAILURE, "Invalid option %s\nTry '%s --help' for more information\n", argv[i_count], NAME);
            }
            
         }
         if (argv[i_count][1] != 0) 
         {
            for (i_index = i_count; i_index < argc - 1; i_index++) 
               argv[i_index] = argv[i_index + 1];
            argc--; i_count--;
         }
      }
   }
  
   if (!i_status)  /* Check for errors parsing the command line */
   {
      if (argc == 2)
      {
         if (isnumeric(argv[1])) 
         {
            i_year = atoi(argv[1]);
            if (i_year > 1752)  /* Check year is valid */
               v_print_months(1, 12, i_year);  /* Display all 12 months in the year */
            else
               printf("Year out of range.\n");
         }
         else
            printf("Argument is not numeric.\n");
      }
      else if (argc == 3)
      {
         if ((isnumeric(argv[1]) && isnumeric(argv[2])))
         {
            i_month = atoi(argv[1]);
            i_year = atoi(argv[2]);
            if (i_month > 0 && i_month < 13 && i_year > 1752)  /* Check month and year are valid */
               v_print_months(i_month, i_months, i_year);
            else
               printf("Year or month out of range.\n");
         }
         else
            printf("Argument is not numeric.\n");
      }   
      else
         printf("Usage: cal [month] year\n");
   }
   return i_status;
}
