/*
 * sdc-maze.c
 *
 * Copyright(C) 2026   MT
 *
 * ASCII maze generator.
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
 * 16 Oct 26   0.1   - Initial version - MT
 *                   - Select a random entrance and exit - MT
 * 17 Oct 26         - Prompt the user to enter a random seed - MT
 *
 * To Do             -
 *
 */

#define  NAME        "sdc-maze"
#define  VERSION     "0.1"
#define  BUILD       "0003"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#define  NODEBUG

#include <stdio.h>   /* Provides printf() for printing output */
#include <stdlib.h>  /* Provides rand()/srand() for random maze generation */
#include <time.h>    /* Provides time() for random number seeding */

#define  HEIGHT 10   /* Number of maze rows */
#define  WIDTH  10   /* Number of maze columns */

#define  NORTH 1     /* Bit flag indicating a north wall */
#define  EAST  2     /* Bit flag indicating an east wall */
#define  SOUTH 4     /* Bit flag indicating a south wall */
#define  WEST  8     /* Bit flag indicating a west wall */
#define  WALLS 15    /* All four walls present */

#define  BUFFER_SIZE 16

#ifndef  true
#define  true 1
#endif
#ifndef  false
#define  false 0
#endif

#ifndef  debug        /* Don't redefine macro if already defined. */
#if defined(DEBUG)
#define debug(code) do {printf("Debug\t: %s line : %d : ", \
            __FILE__, __LINE__); code;} while(0)
#else
#define debug(code)
#endif
#endif


#ifndef stdin  /* If stdin is not defined in stdio define it and fgets() */
#define stdin 0
typedef struct _IO_FILE FILE;
char *fgets(char *s_buffer, int i_length, FILE *h_stream);  
#endif


typedef struct {
   int i_row;        /* Row position of a maze cell */
   int i_column;     /* Column position of a maze cell */
} Cord;

int i_walls[HEIGHT][WIDTH];    /* Wall flags for each cell */
int i_visited[HEIGHT][WIDTH];  /* Records which cells have been visited */
Cord i_stack[HEIGHT * WIDTH];  /* Stack used for depth-first search */
int i_top;                     /* Number of entries currently on the stack */

int i_direction_row[4] = { -1, 0, 1, 0 };  /* Row offsets: north, east, south, west */
int i_direction_column[4] = { 0, 1, 0, -1 };  /* Column offsets: north, east, south, west */

int i_wall_bit[4] = { NORTH, EAST, SOUTH, WEST };
int i_opposite[4] = { SOUTH, WEST, NORTH, EAST };

/* Return true if a cell is inside the maze and has not been visited. */

int b_unvisited(int i_row, int i_column)
{
   if (i_row < 0 || i_row >= HEIGHT ||
       i_column < 0 || i_column >= WIDTH) {
      return false;  /* Outside the maze */
   }

   if (i_visited[i_row][i_column]) {
      return false;  /* Cell has already been visited */
   }

   return true;
}

/* Initialise the maze with all walls intact. */

void v_init_maze(void)
{
   int i_row, i_column;

   for (i_row = 0; i_row < HEIGHT; i_row++)
      for (i_column = 0; i_column < WIDTH; i_column++) {
         i_walls[i_row][i_column] = WALLS;
         i_visited[i_row][i_column] = false;
      }

   i_top = 0;
}

/* Generate a maze using randomized depth-first search and backtracking. */

void v_generate_maze(void)
{
   int i_row, i_column;
   int i_next_row, i_next_column;
   int i_direction;
   int i_directions[4];
   int i_count, i_choice, i_temp;

   /* Choose a random starting cell. */

   i_row = rand() % HEIGHT;
   i_column = rand() % WIDTH;

   i_visited[i_row][i_column] = true;

   i_stack[i_top].i_row = i_row;
   i_stack[i_top].i_column = i_column;
   i_top++;

   /* Continue until every cell has been visited. */

   while (i_top > 0)
   {
      /* Work from the cell at the top of the stack. */

      i_row = i_stack[i_top - 1].i_row;
      i_column = i_stack[i_top - 1].i_column;

      i_count = 0;

      /* Find all unvisited neighbouring cells. */

      for (i_direction = 0; i_direction < 4; i_direction++)
      {
         i_next_row = i_row + i_direction_row[i_direction];
         i_next_column = i_column + i_direction_column[i_direction];

         if (b_unvisited(i_next_row, i_next_column))
            i_directions[i_count++] = i_direction;
      }

      if (i_count == 0)
      {
         /* No unvisited neighbours: backtrack to the previous cell. */

         i_top--;
      }
      else
      {
         /* Shuffle the available directions to add randomness. */

         for (i_choice = i_count - 1; i_choice > 0; i_choice--)
         {
            i_temp = rand() % (i_choice + 1);

            i_direction = i_directions[i_choice];
            i_directions[i_choice] = i_directions[i_temp];
            i_directions[i_temp] = i_direction;
         }

         /* Select the first available direction. */

         i_direction = i_directions[0];

         i_next_row = i_row + i_direction_row[i_direction];
         i_next_column = i_column + i_direction_column[i_direction];

         /* Remove the wall between the current cell and its neighbour. */

         i_walls[i_row][i_column] &= ~i_wall_bit[i_direction];
         i_walls[i_next_row][i_next_column] &= ~i_opposite[i_direction];

         /* Mark the new cell as visited and push it onto the stack. */

         i_visited[i_next_row][i_next_column] = true;

         i_stack[i_top].i_row = i_next_row;
         i_stack[i_top].i_column = i_next_column;
         i_top++;
      }
   }
   
   /* Select a random entrance and exit position */
   
   i_row = rand() % HEIGHT;
   i_walls[i_row][0] &= ~WEST;
   i_row = rand() % HEIGHT;
   i_walls[i_row][WIDTH - 1] &= ~EAST;
}

/* Draw the maze using ASCII characters. */

void v_draw_maze(void)
{
   int i_row, i_column;

   /* Draw the top boundary. */

   for (i_column = 0; i_column < WIDTH; i_column++)
      printf("+---");
   printf("+\n");

   /* Draw each row with left and right walls. */

   for (i_row = 0; i_row < HEIGHT; i_row++)
   {
      /* Draw the left wall, leaving the entrance open. */

      if (i_walls[i_row][0] & WEST)
         putchar('|');
      else
         putchar(' ');

      /* Draw the cell interiors and their east walls. */

      for (i_column = 0; i_column < WIDTH; i_column++)
      {
         printf("   ");

         if (i_walls[i_row][i_column] & EAST)
            putchar('|');
         else
            putchar(' ');
      }

      putchar('\n');

      /* Draw the horizontal walls below the cells. */

      for (i_column = 0; i_column < WIDTH; i_column++)
      {
         putchar('+');

         if (i_walls[i_row][i_column] & SOUTH)
            printf("---");
         else
            printf("   ");
      }

      putchar('+');
      putchar('\n');
   }
}

/* Main program entry point. */

int main(void)
{
   char s_buffer[BUFFER_SIZE];
   char *s_text; 
   int i_seed = 1;  
   
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
   debug(printf("Seed : %d\n", i_seed));
   srand(i_seed);

   v_init_maze();      /* Reset walls and visited cells */
   v_generate_maze();  /* Generate the maze */
   v_draw_maze();      /* Print the completed maze */

   return 0;
}
