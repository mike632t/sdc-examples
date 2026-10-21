/*
 *
 * sdc-list.c
 *
 * Copyright(C) 2025   MEJT
 *
 * Creates a linked list structure.  
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * 23 May 25  0.1  001 - Initial version - MT
 *                 002 - Items can be inserted into the list in order - MT
 *            0.2  003 - Create list using strings - MT
 *                 004 - Allocates storage for each string and saves a copy
 *                       of the data with each node - MT
 * 24 May 25  0.3  005 - Made list routines 'generic' using separate helper
 *                       functions to compare different data types or print
 *                       the values (not unlike the qsort() function)- MT
 *                 006 - Modified to use exit() - MT
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node t_node;

struct node {
   void *data;
   t_node *next;
};

int i_cmpint(const void *h_left, const void *h_right)
{
   int i_left = *(int *)h_left;
   int i_right = *(int *)h_right;
   return (i_left > i_right) - (i_left < i_right);
}

int i_cmpstr(const void *h_left, const void *h_right)
{
   return strcmp((char *)h_left, (char *)h_right);
}

void v_prtint(void *v_data) {
   printf(" %d", *(int *)v_data);
}

void v_prtstr(void *v_data) {
   printf(" %s", (char *)v_data);
}

t_node *h_new(void *h_data, size_t t_size) /* Create a new node */
{
   t_node *h_node;
   h_node = (t_node*)malloc(sizeof(*h_node)); 
   h_node->data = malloc(t_size);
   memcpy(h_node->data, h_data, t_size);
   h_node->next = NULL;
   return h_node;
}

t_node *h_prepend(t_node *h_list, void *h_data, size_t t_size) /* Insert item at head of list */
{
   t_node *h_node = h_new(h_data, t_size); /* Create node */
   h_node->next = h_list;
   return h_node;
}

t_node *h_append(t_node *h_list, void *h_data, size_t t_size) /* Append item to list */
{
   if (h_list != NULL)
      h_list->next = h_append(h_list->next, h_data, t_size);
   else
      h_list = h_new(h_data, t_size); /* Create node */
   return h_list;
}

t_node* h_insert(t_node *h_list, void *h_data, size_t t_size, int (*v_compare)(const void *, const void *)) /* Insert item into list */
{
   if (h_list != NULL)
   {
      if ((v_compare(h_list->data, h_data)) > 0) 
         h_list = h_prepend(h_list, h_data, t_size);
      else
         h_list->next = h_insert(h_list->next, h_data, t_size, v_compare);
   }
   else
   {
      h_list = h_new(h_data, t_size); /* Create node */
   }
   return h_list;
}

t_node *h_delete(t_node *h_list, void *h_data, int (*v_compare)(const void *, const void *))  /* Delete matching item from list */
{
   t_node *h_curr = h_list;
   t_node *h_prev = NULL;

   while (h_curr != NULL)
   {
      if (v_compare(h_curr->data, h_data) == 0)
      {
         if (h_prev == NULL)
            h_list = h_curr->next;  /* Remove from head position */
         else
            h_prev->next = h_curr->next;  /* Bypass current node */

         free(h_curr->data);  /* Free string data area allocation */
         free(h_curr);  /* Free structural node descriptor container */
         return h_list;  /* Match found and deleted successfully */
      }
      h_prev = h_curr;
      h_curr = h_curr->next;
   }
   return h_list;  /* Target item was not present */
}

void v_print(t_node *h_list, void (*v_helper)(void *)) /* Print list */
{
   while (h_list != NULL)
   {
      (*v_helper)(h_list->data);
      h_list = h_list->next;
   }
   printf("\n");
}

int main() {

   t_node *h_list;
   int i_size;
   size_t t_size;

   int i_values[] = {5, 1, 3, 8, 4, 9, 6, 2, 7};
   const char *s_known[] = {"quick", "brown", "jumped", "over", "the"}; 
   const char *s_bad[] = {"error", "bad value"};

   /* Create a (sorted) list of integers */
   h_list = NULL; /* Create an empty list! */
   t_size = sizeof(i_values[0]);  /* Get size of integer values */
   i_size = sizeof(i_values) / t_size;
   for (int i_count = 0; i_count < i_size; ++i_count)
      h_list = h_insert(h_list, &i_values[i_count], t_size, i_cmpint); /* Insert in order */

   i_values[0] = 0;  /* Overwrite data to show storage separation */
   h_list = h_append(h_list, &i_values[0], t_size); /* Append to list */
   v_print(h_list, v_prtint); /* Print list */

   /* Create a list of strings */
   h_list = NULL; /* Another empty list! */
   
   /* Append the known words into the sentence */
   i_size = sizeof(s_known) / sizeof(s_known[0]);
   for (int i_count = 0; i_count < i_size; ++i_count)
      h_list = h_append(h_list, s_known[i_count], strlen(s_known[i_count]) + 1);  /* Capture string with null terminator */

   /* Add a couple extra words to be beginning and end of the list */
   h_list = h_prepend(h_list, s_bad[0], strlen(s_bad[0]) + 1);  /* Inject at head position */
   h_list = h_append(h_list, s_bad[1], strlen(s_bad[1]) + 1);  /* Inject at tail position */

   /* Prepend the first word to the list */
   h_list = h_prepend(h_list, "The", 4); /* Prepend base string */

   /* Delete any unwanted words from the list */
   h_list = h_delete(h_list, s_bad[0], i_cmpstr);  /* Erase head entry */
   h_list = h_delete(h_list, s_bad[1], i_cmpstr);  /* Erase tail entry */


   /* Insert the word 'fox' after the word 'brown'  */
   t_node *h_curr = h_list;
   while (h_curr != NULL)
   {
      if (strcmp((char *)h_curr->data, "brown") == 0)  /* Search the list for the word 'brown' */
      {
         t_node *h_fox = h_new("fox", 4);  /* Generate middle element */
         h_fox->next = h_curr->next;
         h_curr->next = h_fox;  /* Insert after brown */
      }
      h_curr = h_curr->next;
   }

   /* Append the remaining words to the sentence */
   h_list = h_append(h_list, "lazy", 5);
   h_list = h_append(h_list, "dog", 4);

   /* Change one of the original words */
   s_known[1] = "red"; 

   /* Print the final sentence */
   v_print(h_list, v_prtstr); /* Print list */

   return 0; 
}
