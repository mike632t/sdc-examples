;
;   crt0.s
;
;   Modified crt0.s for generic CP/M.
;
;   sdasz80 -o sdc-crt0.s
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
;** 20 Aug 23   - Initial version - MT
;
;** 01 Sep 23   - Added  a  check for the CPU type using the overflow  flag
;		  which behaves differently on the 8080 - MT
;
;** 06 Sep 23	- Defined a seperate stack for the program (otherwise using
;                 functions like printf() on CP/M causes it to crash) - MT
;
;** 12 Sep 23	- Moved defination of putchar() and bdos() here as it seems
;		  to make sense for these to be defined in the runtime code
;		  as  it avoids having to include any CP/M specific  header
;                 files allowing C programs to be compiled with either sdcc
;		  or gcc without any modification - MT
;
;** To Do	- Tidy up code.
;
		.module	crt0

		.globl	_main

		.area	_HEADER (ABS)
		.org	0x0100

		ld	a,#0x7f		; Load with largest positive signed value.
		inc	a		; Incrementing should result in an overflow.
		jp	pe,init		; Z80 processor set the parity flag to signify overflow (8080 doesn't).
		ld	de,#err_msg	; Display error message.
		ld	c,#0x09		; Print string.
		jp	0x0005		; Jump to BDOS (when BDOS returns program will exit).
err_msg:
		.str	"Z80 processor required."
		.db	13,10,'$'
init:
		ld 	(stack),sp	; Save the stack pointer.
		ld	sp,#stack
		call	_main		; Call the C main routine
		ld	sp,(stack)	; Restore original stack pointer
		ret
_bdos::
		ld	c,a		; DE already points to string!
		call	#5
		ex	de,hl		; Return result in DE
		ret
_putchar::
		ex	de,hl
		ld	a,e
		sub	a,#0x0a
		or	a,d
		jr	nz,nolf
		push	de
		ld	de,#0x000d
		ld	a,#0x02
		call	_bdos
		pop	de
nolf:
		ld	a,#0x02
		call	_bdos
		ld	de,#0x0000	; return 0.
		ret
;		.area	_TPA		; Ordering of segments for the linker.
;		.area	_HOME
		.area	_CODE
		.area	_DATA
		.ds	256
stack:	
		.dw	0
;
