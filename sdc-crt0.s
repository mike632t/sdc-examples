;
;   crt0.s
;
;   Modified crt0.s for generic CP/M.
;
;   This program is free software: you can redistribute it and/or modify it
;   under  the terms of the GNU General Public License as published by  the
;   Free Software Foundation, either version 3 of the License, or (at  your
;   option) any later version.
;
;-- This  program  is distributed in the hope that it will be  useful,  but
;   WITHOUT   ANY   WARRANTY;  without  even  the   implied   warranty   of
;   MERCHANTABILITY  or  FITNESS  FOR A PARTICULAR  PURPOSE.  See  the  GNU
;   General Public License for more details.
;
;   You should have received a copy of the GNU General Public License along
;   with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;** 20 Aug 23   0.1   - Initial version - MT
;
;** 01 Sep 23   0.2   - Added  a check for the CPU type using the  overflow
;                       flag which behaves differently on the 8080 - MT
;
;** 24 Nov 24         - Inserted a jump instruction at the beginning of the 
;                       program and tidied up the comments - MT
;
;** 24 Sep 26         - Allocate 128 bytes stack space below the heap - MT
;
;** 26 Sep 26         - Increased stack size to 192 bytes - MT
;
;
                .module crt0
;
                .globl  _main
;
                .area   _HEADER (ABS)
                .org    0x0100
;
;-- Check CPU type (as sdcc requires a Z80).
;
                jp      start           ; Jump the start of program.
;
err_msg:
                .str    "Z80 processor required."
                .db     13,10,'$'
;
start:          ld      a,#0x7f         ; Load with largest positive signed value.
                inc     a               ; Incrementing should result in an overflow.
                jp      pe,init         ; Z80 processor set the parity flag to signify overflow (8080 doesn't).
                ld      de,#err_msg     ; Display error message.
                ld      c,#0x09         ; Print string.
                jp      0x0005          ; Jump to BDOS (when BDOS returns program will exit).
;
init:           ld      (stack),sp      ; Save the stack pointer.
                ld      sp,#stack
                call    _main           ; Call main().
                ld      sp,(stack)      ; Restore original stack pointer
                ret                     ; and return.
;
;-- Place data after program code, and heap after data
;
                .area   _CODE           ; Program code area
                .area   _DATA           ; Data area
                .ds     192             ; Stack space 128 bytes.
stack:          .dw     0
_heap_top::     .dw     0               ; Address of the start of the heap area
;
_HEAP_start::                           ; Heap space.
;
