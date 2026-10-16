/*
 * sdc-snake.c
 *
 * Copyright(C) 2023   MT
 *
 * ASCII turn based snake game.
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
 * with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *
 * 15 Oct 26   0.1   - Initial version - MT
 * 
 * To Do             -
 *
 */

#define  NAME        "sdc-snake"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#include "stdio.h"   /* Provides printf() for printing output */
#include <stdlib.h>  /* rand()/srand() for randomly placing food */

#define  ROWS 11     /* Board height */
#define  COLS 21     /* Board width */
#define  BORDER 2    /* Food must be placed inside a 2 cell border */
#define  LENGTH 35   /* Longest the snake can grow to, sizes the snake[] array */

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#ifndef debug        /* Don't redefine macro if already defined. */
#if defined(DEBUG)
#define debug(code) do {printf("Debug\t: %s line : %d : ", \
            __FILE__, __LINE__); code;} while(0)
#else
#define debug(code)
#endif
#endif

typedef struct {
   int i_row;        /* Row position of one snake segment on the grid */
   int i_col;        /* Column position of one snake segment on the grid */
} Cord;

Cord i_snake[LENGTH];/* Coordinates of each body segment */
int i_length;        /* How many of snake[]'s slots currently hold real segments */
int i_food_row;      /* Current position of the food on the board */
int i_food_column;   /* Current position of the food on the board */
int i_score;         /* Player's score increases each time food is eaten */

void v_draw_board(void)
{
   char c_grid[ROWS][COLS];  /* Scratch grid */
   int i_count, i_row, i_column;

   /* Fill in empty board */
   
   for (i_row = 0; i_row < ROWS; i_row++)
      for (i_column = 0; i_column < COLS; i_column++) 
         c_grid[i_row][i_column] = '.';

   /* Add food and snake */
   
   c_grid[i_food_row][i_food_column] = '*';  /* Mark the food's current cell */

   for (i_count = 0; i_count < i_length; i_count++) 
      if (i_count == 0)  /* Head is '@', every other segment is '#' */
         c_grid[i_snake[i_count].i_row][i_snake[i_count].i_col] = '@';
      else
         c_grid[i_snake[i_count].i_row][i_snake[i_count].i_col] = '#'; 

   /* Draw the top border */
   
   putchar('\n');
   for (i_column = 0; i_column < COLS + 2; i_column++) 
      if (i_column == 0 || i_column == COLS + 1) 
         putchar('+');
      else
         putchar('-');
   putchar('\n');

   /* Draw each row with left and right borders */
   
   for (i_row = 0; i_row < ROWS; i_row++) {
      putchar('|');
      for (i_column = 0; i_column < COLS; i_column++) 
         putchar(c_grid[i_row][i_column]);  /* Print the finished grid one character at a time */
      putchar('|');
      putchar('\n');  /* End of row */
   }

   /* Draw the bottom border */
   
   for (i_column = 0; i_column < COLS + 2; i_column++) {
      putchar((i_column == 0 || i_column == COLS + 1) ? '+' : '-');
   }
   putchar('\n');

   printf("Score: %d   \nLength: %d\n\n", i_score, i_length);  /* Status line under the board */
}

int b_collision(int i_row, int i_column, int i_start)
{
   int i_count;

   for (i_count = i_start; i_count < i_length; i_count++)  /* Start at zero to include the head or 1 to exclude it */
   {
      if (i_snake[i_count].i_row == i_row &&
          i_snake[i_count].i_col == i_column) {  /* Compare the target cell against every body segment */
         return true;
      }
   }
   return false;  /* No segment occupied this cell */
}

void v_add_food(void)
{
   int i_row, i_column;

   do {
      i_row = BORDER + rand() % (ROWS - 2 * BORDER);  /* Random row inside the playable area, */
      i_column = BORDER + rand() % (COLS - 2 * BORDER);  /* Random column inside the playable area */
   } while (b_collision(i_row, i_column, 0));  /* Reject and retry if the random cell landed on the snake */

   i_food_row = i_row;  /* Commit the chosen position as the new food location */
   i_food_column = i_column;
}

void v_update(char c_keypress, int *i_row, int *i_column)
{
   int i_dr = 0, i_dc = 0;  /* Row/column offset the head should move by this turn */

   switch (c_keypress) {
      case 'w': i_dr = -1; i_dc = 0; break;  /* Up: move one row toward the top of the screen */
      case 's': i_dr = 1;  i_dc = 0; break;  /* Down: move one row toward the bottom */
      case 'a': i_dr = 0;  i_dc = -1; break;  /* Left: move one column left */
      case 'd': i_dr = 0;  i_dc = 1; break;  /* Right: move one column right */
      default:  i_dr = 0;  i_dc = 0; break;  /* Unrecognized key: head stays put this turn */
   }

   *i_row = i_snake[0].i_row + i_dr;  /* Written through the pointer so the caller sees the computed value */
   *i_column = i_snake[0].i_col + i_dc;
}

int b_valid_move(int i_row, int i_column)
{
   if (i_row < 0 || i_row >= ROWS ||
       i_column < 0 || i_column >= COLS) {  /* Outside the playable area */
      return false;
   }

   if (b_collision(i_row, i_column, 1)) {  /* Cell is occupied by the snake's own body */
      return false;
   }
   return true;
}

void v_move(int i_row, int i_column)
{
   int i_count;
   int b_eat;

   b_eat = (i_row == i_food_row && i_column == i_food_column);  /* If head lands on food we can eat */

   if (b_eat)
   {
      if (i_length < LENGTH) i_length++;  /* Grow by one segment (if possible) */
      i_score += 10;
   }

   for (i_count = i_length - 1; i_count > 0; i_count--)  /* Shift position from the tail backward (don't overwrite) */
   {
      i_snake[i_count].i_row = i_snake[i_count - 1].i_row;  /* Each segment takes on the position of the one ahead of it */
      i_snake[i_count].i_col = i_snake[i_count - 1].i_col;
   }

   i_snake[0].i_row = i_row;  /* New position for head */
   i_snake[0].i_col = i_column;

   if (b_eat) v_add_food();  /* Only add food after the existing food has been consumed */
}

int main(void)
{
   char c_keypress;  /* Direction character read from the player each turn */
   int i_row, i_column;  /* Computed position the head would go to this turn */
   int b_run;

   i_length = 1;  /* Initial size of snake */
   i_snake[0].i_row = ROWS / 2;  /* Initial position (middle of the board) */
   i_snake[0].i_col = COLS / 2;
   i_score = 0;

   v_add_food();  /* Position first piece of food before the game starts */
   
   b_run = true;
   while (b_run)  /* Repeat until a collision occurs */
   {
      v_draw_board();
      printf("Move (w/a/s/d): ");

      c_keypress = getchar();
      if (c_keypress == 'w' || c_keypress == 'a' || c_keypress == 's' || c_keypress == 'd')  /* Valid key press */
      {
         v_update(c_keypress, &i_row, &i_column);  /* Update position of head (without moving) */

         if (b_run = b_valid_move(i_row, i_column))  /* Check move is valid */
            v_move(i_row, i_column);  /* Update snake's position */
         else
            printf("\nGame Over!\nScore: %d\n", i_score);
      }
   }
   return 0;
}
