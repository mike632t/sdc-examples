/*
 * sdc-getchar.c
 *
 * Copyright(C) 2023   MT
 *
 * Shows how to use getchar() to read from the keyboard.
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
 * Note - Usign  the original calling convention this program will  require 
 * more stack space than is provided by the standard runtime modules.
 *
 * 08 Oct 26   0.1   - Initial version - MT
 * 
 * To Do             -
 *
 */

#define  NAME        "sdc-life"
#define  VERSION     "0.1"
#define  BUILD       "0001"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#define  ROWS        22
#define  COLUMNS     40

#include <stdio.h>
#include <stdbool.h>

int i_grid[ROWS][COLUMNS];

void v_print(int i_grid[ROWS][COLUMNS])
{
   int i_row, i_col;

   for (i_row = 0; i_row < ROWS; i_row++) {
      for (i_col = 0; i_col < COLUMNS; i_col++) {
         printf("%c ", i_grid[i_row][i_col] ? '#' : '.');
      }
      printf("\n");
   }
}

/*
 * Update the Game of Life grid.
 *
 * i_wrap = 0: Bounded grid; cells outside the grid are dead.
 * i_wrap = 1: Wrap edges; the grid behaves like a torus.
 * 
 */
void v_next(int i_grid[ROWS][COLUMNS], int i_wrap)
{
   static int i_next[ROWS][COLUMNS];
   int i_row, i_col, i_x, i_y, i_neighbours;
   int i_r, i_c;

   for (i_row = 0; i_row < ROWS; i_row++) 
   {
      for (i_col = 0; i_col < COLUMNS; i_col++) 
      {
         i_neighbours = 0;  /* Reset count */
         for (i_x = -1; i_x <= 1; i_x++)  /* Count neighbouring cells. */
         {
            for (i_y = -1; i_y <= 1; i_y++) 
            {
               if (i_x != 0 || i_y != 0)  /* Do not count the cell itself. */
               {
                  i_r = i_row + i_x;
                  i_c = i_col + i_y;

                  if (i_wrap)  /* Wrap coordinates around the grid edges. */
                  {
                     i_r = (i_r + ROWS) % ROWS;
                     i_c = (i_c + COLUMNS) % COLUMNS;
                     i_neighbours += i_grid[i_r][i_c];
                  } 
                  else  /* Ignore neighbours outside the grid. */
                  {
                     if (i_r >= 0 && i_r < ROWS &&
                         i_c >= 0 && i_c < COLUMNS) {
                        i_neighbours += i_grid[i_r][i_c];
                     }
                  }
               }
            }
         }

         
         if (i_grid[i_row][i_col])  /* Apply Conway's rules to determine the next state. */
            i_next[i_row][i_col] = (i_neighbours == 2 || i_neighbours == 3);  /* A live cell survives with two or three neighbours. */
         else 
            i_next[i_row][i_col] = (i_neighbours == 3);  /* A dead cell becomes alive with exactly three neighbours. */
      }
   }

   /* Copy the next generation back into the original grid. */
   for (i_row = 0; i_row < ROWS; i_row++) 
      for (i_col = 0; i_col < COLUMNS; i_col++) 
         i_grid[i_row][i_col] = i_next[i_row][i_col];
}

int main(void)
{
   int i_row, i_col;
   int i_generation;

   
   for (i_row = 0; i_row < ROWS; i_row++)  /* Clear all cells */
      for (i_col = 0; i_col < COLUMNS; i_col++) 
         i_grid[i_row][i_col] = 0;
         
//   /* Single glider, centred on the grid */
//   i_grid[ROWS / 2 - 2][COLUMNS / 2]     = true;
//   i_grid[ROWS / 2 - 1][COLUMNS / 2 + 1] = true;
//   i_grid[ROWS / 2][COLUMNS / 2 - 1]     = true;
//   i_grid[ROWS / 2][COLUMNS / 2]         = true;
//   i_grid[ROWS / 2][COLUMNS / 2 + 1]     = true;

//   /* R-pentomino, centred on the grid. */
//   i_grid[ROWS / 2 - 1][COLUMNS / 2]     = true;
//   i_grid[ROWS / 2 - 1][COLUMNS / 2 + 1] = true;
//   i_grid[ROWS / 2][COLUMNS / 2 - 1]     = true;
//   i_grid[ROWS / 2][COLUMNS / 2]         = true;
//   i_grid[ROWS / 2 + 1][COLUMNS / 2]     = true;

   /* Acorn, centred on the grid */
   i_grid[ROWS / 2 - 2][COLUMNS / 2 - 1] = true;
   i_grid[ROWS / 2 - 1][COLUMNS / 2 + 1] = true;

   i_grid[ROWS / 2][COLUMNS / 2 - 2]     = true;
   i_grid[ROWS / 2][COLUMNS / 2 - 1]     = true;
   i_grid[ROWS / 2][COLUMNS / 2 + 2]     = true;
   i_grid[ROWS / 2][COLUMNS / 2 + 3]     = true;
   i_grid[ROWS / 2][COLUMNS / 2 + 4]     = true;

   printf("\033[2J");  /* Clear screen */
   for (i_generation = 0; i_generation < 850; i_generation++) 
   {
      printf("\033[H");
      printf("\nGeneration: %d\n\n", i_generation);
      v_print(i_grid);
      v_next(i_grid, true);
   }

   return 0;
}
