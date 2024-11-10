/*
 *
 * sdc-hsv2rgb.c
 *
 * Copyright(C) 2023   MEJT
 *
 * Converts HSV colours to RGB and back again.
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
 *            /|_
 *         ,'   .\
 *     ,--'##\ _,'
 *    / ###  /
 *   (##--.  |
 *   |###  ) |
 *  (`---.'--.)
 *   `--._)---'
 * 
 * https://stackoverflow.com/questions/3018313/
 *
 *  9 Oct 23   0.1      - Initial version - MT
 * 12 Oct 23            - Fixed overflow issues - MT
 * 14 Oct 23            - Tidied up vairable names - MT
 *
 */

#include <stdio.h>                        /* printf(), etc. */
#include <stdlib.h>                       /* exit(), etc. */
#include <string.h>                       /* strlen(), etc */
#include <stdarg.h>                       /* va_start(), va_end(), etc */
#include <stdint.h>
#include <math.h>

#define  NAME           "gcc-hsv2rgb"
#define  VERSION        "0.1"
#define  BUILD          "0001"
#define  AUTHOR         "MT"
#define  DATE           "9 Oct 23"

#define  True           0
#define  False          !True

#define  DEBUG

#ifndef debug /* Don't redefine macro if already defined. */
#if defined(DEBUG)
#define debug(code) do {fprintf(stderr, "Debug\t: %s line : %d : ", \
            __FILE__, __LINE__); code;} while(0)
#else
#define debug(code)
#endif
#endif

uint32_t pack(uint8_t i_a, uint8_t i_b, uint8_t i_c)
{
   return((((((uint32_t)i_a) << 8) + i_b) << 8) + i_c);
}

uint32_t hsv2rgb(uint8_t i_hue, uint8_t i_saturation, uint8_t i_value)
{
   uint16_t i_range, i_remainder;
   uint8_t i_red, i_green, i_blue;
   uint8_t i_p, i_q, i_t;
   
   if (i_saturation == 0)
   {
      i_red = i_value;
      i_green = i_value;
      i_blue = i_value;
      return pack(i_red, i_green, i_blue);
   }
   
   i_range = (uint16_t)i_hue / 43;
   i_remainder = ((uint16_t)i_hue - (i_range * 43)) * 6; 
   
   i_p = ((uint16_t)i_value * (255 - (uint16_t)i_saturation)) >> 8;
   i_q = ((uint16_t)i_value * (255 - (((uint16_t)i_saturation * i_remainder) >> 8))) >> 8;
   i_t = ((uint16_t)i_value * (255 - (((uint16_t)i_saturation * (255 - i_remainder)) >> 8))) >> 8;
   
   switch (i_range)
   {
      case 0:
         i_red = i_value; i_green = i_t; i_blue = i_p;
         break;
      case 1:
         i_red = i_q; i_green = i_value; i_blue = i_p;
         break;
      case 2:
         i_red = i_p; i_green = i_value; i_blue = i_t;
         break;
      case 3:
         i_red = i_p; i_green = i_q; i_blue = i_value;
         break;
      case 4:
         i_red = i_t; i_green = i_p; i_blue = i_value;
         break;
      default:
         i_red = i_value; i_green = i_p; i_blue = i_q;
         break;
   }
   return pack(i_red, i_green, i_blue);
}

uint32_t rgb2hsv(uint8_t i_red, uint8_t i_green, uint8_t i_blue)
{
   uint8_t i_min, i_max;
   uint8_t i_hue, i_saturation, i_value;

   i_min = i_red < i_green ? (i_red < i_blue ? i_red : i_blue) : (i_green < i_blue ? i_green : i_blue);
   i_max = i_red > i_green ? (i_red > i_blue ? i_red : i_blue) : (i_green > i_blue ? i_green : i_blue);
   
   i_value = i_max;
   if (i_value == 0)
   {
      i_hue = 0;
      i_saturation = 0;
   }
	else
	{
		i_saturation = 255 * (uint16_t)(i_max - i_min) / i_value;
		if (i_saturation == 0)
		{
			i_hue = 0;
		}
		else
		{
			if (i_max == i_red)
				i_hue = 0 + 43 * (i_green - i_blue) / (i_max - i_min);
			else if (i_max == i_green)
				i_hue = 85 + 43 * (i_blue - i_red) / (i_max - i_min);
			else
				i_hue = 171 + 43 * (i_red - i_green) / (i_max - i_min);
		}
	}
   return pack(i_hue, i_saturation, i_value);
}

int main()
{
   uint32_t i_rgb, i_hsv;
   uint8_t i_red, i_green, i_blue;
   uint8_t i_hue, i_saturation, i_value;
    
	for (uint16_t i_count = 0; i_count < 256; i_count++) 
   {
      i_rgb = hsv2rgb(i_count, 255,128);
      i_red = (i_rgb >> 16) & 0xff;
      i_green = (i_rgb >> 8)  & 0xff;
      i_blue = i_rgb & 0xff;
      i_hsv = rgb2hsv(i_red, i_green, i_blue);
      i_hue = (i_hsv >> 16)  & 0xff;
      i_saturation = (i_hsv >> 8)  & 0xff;
      i_value = i_hsv & 0xff;
      
      printf("%4d %02x%02x%02x  %02x%02x%02x  %02x%02x%02x\n", i_count, i_count, 255, 128, i_red, i_green, i_blue, i_hue, i_saturation, i_value);
   }
 
   return 0;
}
