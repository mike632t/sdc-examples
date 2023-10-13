/*
 * sdc-mandlebrot.c
 *
 * Copyright(C) 2023   MEJT
 *
 * Plots a mandlebrot set using non standard ANSI escape seqences.
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
 * https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set
 * https://stackoverflow.com/questions/3018313/
 *
 * 
 * 03 Sep 23            - Initial version - MT
 * 07 Oct 23   0.1      - Replaced the simple character mapping with a more
 *                        complex function that generates an RGB colour and
 *                        use  it to plot the mandlebrot set using extended
 *                        escape sequences - MT
 *                      - Increased number of iterations to 64 - MT
 * 12 Oct 23            - Fixed overflow issues - MT
 *
 */

#include <stdio.h>                        /* printf(), etc. */
#include <stdlib.h>                       /* exit(), etc. */
#include <string.h>                       /* strlen(), etc */
#include <stdarg.h>                       /* va_start(), va_end(), etc */
#include <stdint.h>
#include <math.h>

#define  NAME           "sdc-ansi-mandlebrot"
#define  VERSION        "0.1"
#define  BUILD          "0001"
#define  AUTHOR         "MT"
#define  DATE           "26 Sep 23"

#define  WIDTH          132               /* Fallback terminal size */
#define  HEIGHT         34

#define  True           0
#define  False          !True

#if defined(DEBUG)
#define debug(code) do {printf("Debug\t: %s line : %d : ", \
            __FILE__, __LINE__); code;} while(0)
#endif

void v_version() /* Display version information */
{
   printf("%s: Version %s", NAME, VERSION);
   if (__DATE__[4] == ' ') printf(" 0"); else printf(" %c", __DATE__[4]);
   printf("%c %c%c%c %s %s", __DATE__[5],
      __DATE__[0], __DATE__[1], __DATE__[2], &__DATE__[9], __TIME__ );
   printf(" (Build: %s)\n", BUILD );
}

void v_about() /* Display help text */
{
   printf("Usage: %s [OPTION]...\n", NAME);
   printf("Display Mandlebrot or Juila set using ASCII characters.\n\n");
   printf("  -?, --help               display this help and exit\n");
   printf("      --version            output version information and exit\n");
}

void v_error(const char *s_format, ...) /* Print formatted error message and exit */
{
   va_list t_args;
   va_start(t_args, s_format);
   printf("%s : ", __FILE__);
   vprintf(s_format, t_args);
   va_end(t_args);
}

uint32_t pack(uint8_t a, uint8_t b, uint8_t c)
{
   return((((((uint32_t)a) << 8) + b) << 8) + c);
}

uint32_t hsv2rgb(uint8_t h, uint8_t s, uint8_t v)
{
   uint16_t i, m;
   uint8_t r, g, b;
   uint8_t p, q, t;
   
   if (s == 0)
   {
      r = v;
      g = v;
      b = v;
      return pack(r, g, b);
   }
   
   i = (uint16_t)h / 43;
   m = ((uint16_t)h - (i * 43)) * 6; 
   
   p = ((uint16_t)v * (255 - (uint16_t)s)) >> 8;
   q = ((uint16_t)v * (255 - (((uint16_t)s * m) >> 8))) >> 8;
   t = ((uint16_t)v * (255 - (((uint16_t)s * (255 - m)) >> 8))) >> 8;
   
   switch (i)
   {
      case 0:
         r = v; g = t; b = p;
         break;
      case 1:
         r = q; g = v; b = p;
         break;
      case 2:
         r = p; g = v; b = t;
         break;
      case 3:
         r = p; g = q; b = v;
         break;
      case 4:
         r = t; g = p; b = v;
         break;
      default:
         r = v; g = p; b = q;
         break;
   }
   return pack(r, g, b);
}

int v_draw_mandlebrot_set(int i_height, int i_width)
{
   const float f_xmin = -2.25;            /* Left edge      */
   const float f_xmax = 0.75;             /* Right edge     */
   const float f_ymin = -1.25;            /* Top edge       */
   const float f_ymax = 1.25;             /* Bottom edge    */
   const int i_maxiteration = 64;         /* Iterations     */

   float f_xdelta;                        /* X step size    */
   float f_ydelta;                        /* Y step size    */

   float cr, ci;
   float zr, zi, temp;
   float r = 2.0;                         /* Radius         */
   
   uint16_t x, y;
   uint8_t i;
   uint32_t i_colour;

   f_xdelta = (f_xmin - f_xmax) / i_width;
   f_ydelta = (f_ymin - f_ymax) / i_height;

   for (y = 0; y < i_height; y++)
   {
      if (y > 0 ) printf("\n");
      for (x = 0; x < i_width; x++)
      {
         cr = f_xmin - (x * f_xdelta);
         ci = f_ymin - (y * f_ydelta);
         zr = 0.0;
         zi = 0.0;
         i = 0;
         while ((((zr * zr) + (zi * zi)) < (r * r)) && (i < i_maxiteration))
         {
            temp = zr * zr - zi * zi;
            zi = 2 * zr * zi + ci;
            zr = temp + cr;
            i++;
         }
         if (i == i_maxiteration)
            printf("\033[48;2;0;0;0m ");
         else
         {
            i = 255 * ((float)i / i_maxiteration);
            i_colour = hsv2rgb(i, 255, 128);
            printf("\033[48;2;%03d;%03d;%03dm ", (uint8_t)(i_colour >> 16)  & 0xff, (uint8_t)(i_colour >> 8)  & 0xff, (uint8_t)i_colour & 0xff);
         }
      }
      printf("\033[0m");
   }
   return True;
}

int main(int argc, char *argv[])
{
   int i_width = WIDTH;
   int i_height = HEIGHT;

   int i_count, i_index;
   char b_abort = False; /* Stop processing command line */

   for (i_count = 1; i_count < argc && (b_abort != True); i_count++)
   {
      if (argv[i_count][0] == '-')
      {
         i_index = 1;
         while (argv[i_count][i_index] != 0)
         {
            switch (argv[i_count][i_index])
            {
               case '?': /* Display help */
                  v_about();
                  return 0;
               case '-': /* '--' terminates command line processing */
                  i_index = strlen(argv[i_count]);
                  if (i_index == 2)
                    b_abort = True; /* '--' terminates command line processing */
                  else
                  {
                     if (!strncmp(argv[i_count], "--version", i_index))
                     {
                        v_version(); /* Display version information */
                        return 0;
                     }
                     else if (!strncmp(argv[i_count], "--help", i_index))
                     {
                        v_about();
                     }
                     else /* If we get here then the we have an invalid long option */
                     {
                        v_error("%s: invalid option %s\nTry '%s --help' for more information.\n", argv[i_count][i_index] , NAME);
                        return -1;
                     }
                  }
                  i_index--; /* Leave index pointing at end of string (so argv[i_count][i_index] = 0) */
                  break;
               default: /* If we get here the single letter option is unknown */
                  v_error("unknown option -- %c\nTry '%s --help' for more information.\n", argv[i_count][i_index] , NAME);
                  return -1;
            }
            i_index++; /* Parse next letter in options */
         }
         if (argv[i_count][1] != 0) {
            for (i_index = i_count; i_index < argc - 1; i_index++) argv[i_index] = argv[i_index + 1];
            argc--; i_count--;
         }
      }
   }
   v_draw_mandlebrot_set(i_height, i_width);
   getchar();
   printf("\n");
   return 0;
}
