/*
 * sdc-gets.c
 *
 * Copyright(C) 2026   MT
 *
 * Shows how to use fgets() to allow users to input text and integer values
 * from the console.
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
 * 17 Oct 26   0.1   - Initial version - MT
 *
 * To Do             -
 *
 */

#define  NAME        "sdc-gets"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#include "stdio.h"   /* printf() */
#include <stdlib.h>  /* strtoul() */
#include <string.h>  /* strlen(), strcspn() */
#include <stdbool.h> /* True/False */

#define BUFFER_SIZE 16

#ifndef stdin
#define stdin 0
typedef struct _IO_FILE FILE;
#endif

char *fgets(char *s_buffer, int i_length, FILE *h_stream);

void main(void) 
{
   char s_buffer[BUFFER_SIZE];
   char *s_text; 
   int i_seed = 1;  
   
   /* Prompt user to enter a name and repeat until an empty string it entered */
   
   while (true)
   {
      printf("Enter name : ");
      if (fgets(s_buffer, sizeof(s_buffer), stdin) == NULL || s_buffer[0] == '\n')  /* If user enters an empty string continue */ 
         break;
      /** s_buffer[strcspn(s_buffer, "\n")] = '\0';  /* Remove trailing newline character */
      if ((s_text = strchr(s_buffer, '\n')) != NULL) *s_text = '\0';  /* Remove trailing newline character (strchr() returns NULL if '\n' not found) */
      printf("Name : '%s'\n", s_buffer);
   }

   /* Prompt user to enter a random seed and repeat until a non zero numeric value is entered */
   
   while (true)
   {
      printf("Enter random seed : ");
      if ((fgets(s_buffer, sizeof(s_buffer), stdin)) == NULL)  /* Check for EOF */
         break;
      i_seed = (int)strtoul(s_buffer, &s_text, 0);
      if (i_seed != 0 && (*s_text == '\n' || *s_text == '\0'))
         break;
      printf("Error: Invalid value.\n");
   } 
   printf("Seed : %d\n", i_seed);
}
