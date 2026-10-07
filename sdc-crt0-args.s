;
;   crt0-args.s
;
;   Copyright(C) 2023   MT
;
;-- Enhanced crt0.s for generic CP/M that copies the command line arguments
;   from the command tail into argv[] and passes argc and argv[] to main.
;
;   This allows main() to be defined as 'int main(int argc, char* argv[])'.
;
;   CP/M stores the length of the command line in memory at 0080H, followed
;   by the command line itself which may be up to 127 characters long.  
;
;   To save memory the pointers to each argument are written to memory from
;   0x0100, overwriting the now redundant CPU check which gives the program
;   enough space to hold up to sixteen array elements.
;
;-- This program is free software: you can redistribute it and/or modify it
;   under  the terms of the GNU General Public License as published by  the
;   Free Software Foundation, either version 3 of the License, or (at  your
;   option) any later version.
;
;   This  program  is distributed in the hope that it will be  useful,  but
;   WITHOUT   ANY   WARRANTY;  without  even  the   implied   warranty   of
;   MERCHANTABILITY  or  FITNESS  FOR A PARTICULAR  PURPOSE.  See  the  GNU
;   General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;** 22 Nov 24   0.1   - Initial version - MT 
;                     - Added check CPU type - MT
;                     - Arguments  are now now ANSI compliant (returning  a 
;                       null string for arg[0] is valid) - MT
;
;** 24 Nov 24         - Exit  via a CP/M reset to avoid any stack  overflow 
;                       issues - MT
;                     - Inserted a jump instruction at the beginning of the 
;                       program and tidied up the comments - MT
;
;** 24 Sep 26         - Allocate 512 bytes stack space below the heap. This
;                       should provide enough stack space for most programs
;                       and avoiding the need to exit via a CP/M reset - MT
;
;** 25 Sep 26         - Added support for global variables - MT
;
;** 27 Sep 26         - Fixed bug in stack pointer - MT
;
;** 07 Oct 26         - Moved command line parser to sdc-cpm.c to allow the
;                       compiler to select the calling convention - MT
;
;   To Do:            - 
;
;
Asc$NUL         .equ    0x00            ; <Ctrl/@> - Null.
Asc$SP          .equ    0x20            ; Space.
;
CPM$Buff        .equ    0x0080          ; Start of CP/M file buffer 80..FFH.
CPM$Boot        .equ    0x0000          ; BIOS reboot.
CPM$Load        .equ    0x0100          ; Program load address.
CPM$BDOS        .equ    0x0005
;
BDOS$PrtStr     .equ    0x09            ; Print a string to CON:.

                .module crt0
;
                .globl  _main
                .globl  l__INITIALIZER
                .globl  s__INITIALIZER
                .globl  s__INITIALIZED
                .globl  ___parse
                .globl  ___sdcc_heap_init
;
                .area   _HEADER (ABS)
                .org    CPM$Load
;
;-- Check CPU type (as sdcc requires a Z80).
;
                jp      start           ; Jump the start of program.
;
error:          .str    "Z80 processor required."
                .db     13,10,'$'
;
start:          ld      a,#0x7f         ; Load with largest positive signed value.
                inc     a               ; Incrementing should result in an overflow.
                jp      pe,init         ; Z80 processor set the parity flag to signify overflow (8080 doesn't).
                ld      de,#error       ; Display error message.
                ld      c,#BDOS$PrtStr  ; Print string.
                jp      CPM$BDOS        ; Jump to BDOS (when BDOS returns program will exit).
;
;-- Set up stack and initialize static/global variables.
;
init:           ld      bc,#l__INITIALIZER
                ld      a,b
                or      a,c
                jr      z,done          ; Nothing to do here.
                ld      de,#s__INITIALIZED
                ld      hl,#s__INITIALIZER
                ldir                    ; Copy initial values to memory.
;
;-- Parse the command line.
;
done:           ld      (stack),sp      ; Save the stack pointer.
                ld      sp,#stack
                call    ___sdcc_heap_init 
                call    ___parse        ; Parser calls main()
;
;-- Exit program when main is finished.
;
                ld      sp,(stack)      ; Restore original stack pointer
                ret                     ; and return.
;
;-- Alternatively perform a warm reset.
;
;               ld      c,#0            ; Call BDOS RESET function
;               jp      5

;
;-- Define order of storage areas (place data after program code).
;
;               .area   _HOME
                .area   _CODE           ; Program code area.
                .area   _INITIALIZER
                .area   _INITIALIZED    ; Global variables.
                .area   _DATA           ; Data area.
                .ds     512             ; Stack space 512 bytes.
stack:          .dw     0
;               .area   _BSS
                .area   _HEAP           ; Place heap after data.
;
