/*
 * sdc-caesar.c - Example program for SDCC.
 *
 * Copyright(C) 2023   MT
 *
 * Implementation  of a simple Ceasar cypher using ROT13 which is a special
 * case of a Caesar cypher as it is it's own inverse. 
 * 
 * This algorithm is often cited as a text book example of weak  encryption
 * as it provides virtually no cryptographic security at all.
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
 * https://www.rosettacode.org/wiki/Caesar_cipher#C
 *
 * 17 Sep 23   0.1   - Initial version adapted from Rosetta Code - MT
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void rot(int i_distance, char *s_text)
{
   int l = strlen(s_text);

   const char* s_lower_case  = "abcdefghijklmnopqrstuvwxyz";
   const char* s_upper_case = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

   int i_offset;

   for (int i_count = 0; i_count < l; i_count++)
   {
      if(isalpha(s_text[i_count]) == 0) continue; 
      i_offset = (int) ((tolower(s_text[i_count]) - 'a') + i_distance) % 26;
      if(isupper(s_text[i_count])) /* Preserve case */
         s_text[i_count] = s_upper_case[i_offset];
      else
         s_text[i_count] = s_lower_case[i_offset];
   }
}

int main()
{
   char s_text[] = "A Ceasar Cypher is a simple substution cypher";

   printf("Plaintext: `%s`\n", s_text);
   rot(13, s_text);
   printf("Encrypted: `%s`\n", s_text);
   rot(13, s_text);
   printf("Decrypted: `%s`\n", s_text);
   return 0;
}
