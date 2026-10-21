/*
 * sdc-cpm.c
 *
 * Copyright(C) 2023   MT
 *
 * Provides minimal support for the CP/M operating system.
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
 * 20 Aug 23   0.1   - Initial version - MT
 * 29 Sep 26   0.2   - Added support for both old and new calling standards
 *                     allowing code to be built without modification using
 *                     either version - MT
 * 07 Oct 26   0.3   - Moved command line parser here to allow the compiler
 *                     to determine the correct calling convention - MT
 *                   - Rewrote the command line parser - MT
 *                   - Parser now supports new calling convention - MT
 * 08 Oct 26   0,4   - Added getchar() - MT
 * 12 Oct 26         - Fixed issue with status - MT
 *                   - Updated BDOS for new calling convention - MT
 * 
 * ToDo              - Allow  command  line parameters to  be  enclosed  in
 *                     quotes.
 *
 */

#define  NAME        "sdc-cpm"
#define  VERSION     "0.4"
#define  BUILD       "0007"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#include "sdc-cpm.h"
#include "stdio.h"   /* Provides EOF */

#if __SDCCCALL == 1

unsigned int bdos(unsigned char c_byte, unsigned int i_word) __naked
{
   c_byte;
   i_word;
   __asm
               ld    c,a               ; Move byte in A to C.
               call  CPM$BDOS          ; Character already in E.
               ld    e,a               ; Return result in DE.
               ld    d,#0
               ret
   __endasm;
}

#else

unsigned int bdos(unsigned char c_byte, unsigned int i_word) __naked
{
   c_byte;
   i_word;
   __asm
               ld    hl,#2             ; Offset into stack.
               add   hl,sp             ; Get address of byte.
               ld    c,(hl)            ; Save BDOS function in C.
               inc   hl                ; Increment pointer.
               ld    e,(hl)            ; Save word in DE (low byte hi byte).
               inc   hl               
               ld    d,(hl)           
               call  CPM$BDOS          ; Return from BDOS.
               ld    l,a
               ld    h,#0
               ret
   __endasm;
}

#endif

unsigned int __parse() __naked  /* Parse command line  */
{
   __asm
;
;-- Terminate command line with an ASCII NUL this will tell us when to stop 
;   scanning and terminate the last argument.
;
               ld    hl,#CPM$Buffer    ; Pointer to command line.
               xor   a                 ; Ignore high byte
               ld    b,a
               ld    c,(hl)            ; Length of command line in BC.
               ld    (hl),a            ; Make argv[0] a null string.
;              
               add   hl,bc             
               inc   hl                ; Offset to end of command line.
               ld    (hl),a            ; Terminate command line with a null.
;
               ld    hl,#CPM$Buffer    ; Reset pointer to command line and
               ld    c,a               ; the number of arguments.
               ld	   de,#CPM$Load+3		; Pointer to argv[0].  
;
;-- Save pointers to argument in argv[] (at 0x103) and increment argc.
;
save:          ex	   de,hl			      ; Store address of string in argv[].
               ld	   (hl),e
               inc	hl			         ; Increment pointer to argv[]
               ld	   (hl),d
               inc	hl			         ; Increment pointer to argv[]
               ex	   de,hl
               inc	bc                ; Incriment argc.
;
skip:          inc   hl                ; Move to next character in buffer.
               ld	   a,(hl)            
               or	   a			         ; Set flags
               jr	   z,done            ; If it is null then we are done.
               cp	   #' '
               jr	   nz,skip           ; Skip spaces
;
               xor   a
               ld    (hl),a            ; Terminate argument with a null,
               inc   hl
               jr    save
;

#if __SDCCCALL == 1
               
;
;-- Call main(argc, argv[])
;
done:          ld	   de,#CPM$Load+3		; Pointer to argv[0].  
               ld    h,b
               ld    l,c
               call  _main
               ret
;
   __endasm;
}

#else

;
;-- Call main(argc, argv[])
;
done:          ld	   hl,#CPM$Load+3		; Pointer to argv[0].  
               push  hl                ; Push in reverse order - argv[] first
               push  bc                ; then argc
               call  _main
               pop   bc                ; Clean up the stack
               pop   hl
               ret   
;
   __endasm;
}

#endif

int putchar(int c_char)
{
   if (c_char == '\n') bdos(BDOS$CON_Output, '\r');
   bdos(BDOS$CON_Output, c_char);
   return 0;
}

int getchar(void)
{
   int i_status;

   while ((i_status = bdos(BDOS$Direct_IO , 0xFF)) == 0) {} /* Wait until a key is pressed */

   if (i_status == Asc$EOF) i_status = EOF;  /* Check for EOF */
  
   return i_status;
}


#define stdin 0
typedef int FILE;

char *fgets(char *s_buffer, int i_length, FILE *h_stream) 
{
   int i_count = 0;
   int i_counter = 0;
   int c_char = '\0';  /* Integer because value  returned from getchar() is an integer (EOF is -1) */
   char *p_result = s_buffer;  /* Default is to return pointer to buffer */

   if (h_stream == 0)
   {
      while (i_count < (i_length - 1)) 
      {
         c_char = getchar();
         if (c_char == Asc$CR || c_char == Asc$LF)  /* Carriage return or linefeed terminate input */
            break;
         else if (c_char == EOF)  
         {
            if (i_count == 0) p_result = NULL;  /* EOF at start of input returns NULL (leaving buffer unmodified) */
            break;
         }
         else if (c_char == Asc$BS || c_char == Asc$NAK)  /* Back space deletes a single character Ctrl-U all preceding characters */
         {
            if (i_count > 0)
            {
               if (c_char == Asc$BS) i_counter = 1; else i_counter = i_count;
               while (i_counter > 0)
               {
                  putchar(Asc$BS);
                  putchar(' ');  /* Erase character on screen */
                  putchar(Asc$BS);
                  i_counter--;
                  s_buffer[--i_count] = Asc$NUL;  /* Erase character from buffer */
               }
            }
         }
         else
         {
            s_buffer[i_count++] = (char)c_char;
            if (c_char >= Asc$SP && c_char < Asc$DEL) putchar(c_char);
         }
      }
      if (p_result != NULL)
      {
         if (c_char != EOF && (i_count < (i_length - 1)))  /* Append newline if space allows and not EOF*/
            s_buffer[i_count++] = Asc$LF;
         s_buffer[i_count] = '\0'; /* Append a string terminator */
      }
      putchar(Asc$CR);
      putchar(Asc$LF);
   }
   else
      p_result = NULL;
   return p_result;
}
