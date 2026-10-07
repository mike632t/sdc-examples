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
 * 
 * ToDo              - Check BDOS return value.
 *                   - Allow  command  line parameters to  be  enclosed  in
 *                     quotes.
 *
 */

#define  NAME        "sdc-cpm"
#define  VERSION     "0.3"
#define  BUILD       "0004"
#define  AUTHOR      "MT"
#define  COPYRIGHT   (__DATE__ + 7) /* Extract copyright year from date. */

#include "sdc-cpm.h"

#if __SDCCCALL == 1

unsigned int bdos(unsigned char c_byte, unsigned int i_word) __naked
{
   c_byte;
   i_word;
   __asm
                ld    c,a               ; Move byte in A to C.
                jp    CPM$BDOS          ; Character already in E.
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
               jp    CPM$BDOS          ; Return from BDOS.
   __endasm;
}

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

int putchar(int c)
{
   if (c == '\n') bdos(BDOS$CON_Output, '\r');
   bdos(BDOS$CON_Output, c);
   return 0;
}
