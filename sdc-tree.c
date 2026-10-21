/*
 *
 * gcc-tree.c
 *
 * Copyright(C) 2025   MEJT
 *
 * Creates a binary tree structure.  
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
 * 20 May 25  0.1  001 - Initial version - MT
 * 25 May 25  0.2  002 - Creates binary tree using strings - MT
 *            0.3  003 - Allocates storage for each string and saves a copy
 *                       of the data with each leaf - MT
 *            0.4  004 - Uses  helper functions to allow binary trees to be 
 *                       created using different data types - MT
 *                 005 - Modified to use exit() - MT
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct leaf
{
   char *data;
   struct leaf *left;
   struct leaf *right;
};

typedef struct leaf leaf;

int i_cmpint(const void *h_left, const void *h_right)
{
   int i_left = *(int *)h_left;
   int i_right = *(int *)h_right;
   return (i_left > i_right) - (i_left < i_right);
}

int i_cmpnum(const void *h_left, const void *h_right)
{
   float f_left = *(float *)h_left;
   float f_right = *(float *)h_right;
   return (f_left > f_right) - (f_left < f_right);
}

int i_cmpstr(const void *h_left, const void *h_right)
{
   return strcmp((char *)h_left, (char *)h_right);
}

void v_prtint(void *v_data)
{
   printf(" %d", *(int *)v_data);
}

void v_prtnum(void *v_data)
{
   printf(" %.5f", *(float *)v_data);
}

void v_prtstr(void *v_data)
{
   printf(" %s", (char *)v_data);
}

leaf *h_new(void *h_data, size_t t_size) /* Create a new leaf */
{
   leaf *h_leaf;
   h_leaf = (leaf*)malloc(sizeof(*h_leaf));
   h_leaf->data = malloc(t_size); /* Allocates storage for data */
   memcpy(h_leaf->data, h_data, t_size); /* and copies it */ 
   h_leaf->left = NULL;
   h_leaf->right = NULL;
   return h_leaf;
}

leaf *h_add(leaf *h_root, void *h_data, size_t t_size, int (*v_compare)(const void *, const void *)) /*  Adds a leaf to the tree */
{
   if (h_root != NULL)
      if ((v_compare(h_root->data, h_data)) > 0) 
         h_root->left = h_add(h_root->left, h_data, t_size, v_compare);
      else
         h_root->right = h_add(h_root->right, h_data, t_size, v_compare);
   else
      h_root = h_new(h_data, t_size); /* Create leaf */
   return h_root;
}

void v_inorder(leaf *h_root, void (*v_helper)(void *)) /* Print list */
{
   if (h_root != NULL)
   {
      v_inorder(h_root->left, v_helper); /* Print left sub-tree */
      (*v_helper)(h_root->data);
      v_inorder(h_root->right, v_helper); /* Print right sub-tree */
   }
}

int main()
{
   leaf *h_root;
   
   int i_size;
   size_t t_size;
   
   const int i_array[] = { 5, 1, 3, 8, 4, 9, 6, 2, 7, 0 };
   const float f_array[] = { 1.61803, 1.73205, 3.14159, 2.71828, 1.41421 };
   const char *s_array[] = { "William", "Michael", "Lisa", "John", "Mary",
                        "David", "James", "Karen", "Robert", "Linda",
                        "Mark",  "Patricia", "Deborah", "Richard",
                        "Elizabeth" };   

   /* Create binary tree of integers */
   h_root = NULL;
   t_size = sizeof(i_array[0]);
   i_size = sizeof(i_array) / t_size;
   for (int i_count = 0; i_count < i_size; ++i_count)
      h_root = h_add(h_root, &i_array[i_count], t_size, i_cmpint);
   v_inorder(h_root, v_prtint);
   printf("\n");

   /* Create binary tree of strings */
   h_root = NULL;
   i_size = sizeof(s_array) / sizeof(s_array[0]);
   for (int i_count = 0; i_count < i_size; ++i_count)
      h_root = h_add(h_root, s_array[i_count], strlen(s_array[i_count]) + 1, i_cmpstr);  /* strlen() does not include terminating NUL */
   v_inorder(h_root, v_prtstr);
   printf("\n");

   /* Create binary tree of floats */
   h_root = NULL;
   t_size = sizeof(f_array[0]);
   i_size = sizeof(f_array) / t_size;
   for (int i_count = 0; i_count < i_size; ++i_count)
      h_root = h_add(h_root, &f_array[i_count], t_size, i_cmpnum);
   v_inorder(h_root, v_prtnum);
   printf("\n");
   
   return 0;
}
