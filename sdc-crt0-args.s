;
;   crt0.s
;
;   Copyright(C) 2023   MT
;
;   Enhanced crt0.s for generic CP/M that copies the command line arguments
;   from the command tail into argv[] and passes argc and argv[] to main.
;
;   Note - This is not compatible with versions of sdcc prior to 4.2.0.
;
;   This allows main() to be defined as 'int main(int argc, char* argv[])'.
;
;   CP/M stores the length of the command line in memory at 0080H, followed
;   by the command line itself which may be up to 127 characters long.  The
;   parser  builds  an array of pointers to each argument and replaces  the
;   spaces after each argument with a null to terminate each string.
;
;   sdasz80 -o sdc-crt0-args.s
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
;               - Checks how many arguments have been parsed to prevent the
;                 array of pointers over writing the parsing code - MT
;               - Uses the memory occupied by the inital code to store  the
;                 pointers to each command line argument, this is sufficent
;                 to handle up to 34 arguments.  Should the command line be
;                 longer  than this any remaining characters will be passed
;                 to main() along with the last argument - MT
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
		.globl _bdos
		.globl _putchar

		.area	_HEADER (ABS)
		.org	0x0100
args:					; Overwriting the initial code with succesive pointers
					; to each command line parameter allows up to 34 arguments
					; be passed to the main program.
		jp	start
err_msg:
		.str	"Z80 processor required."
		.db	00,13,10,'$'
start:
		ld	a,#0x7f		; Load with largest positive signed value.
		inc	a		; Incrementing should result in an overflow.
		jp	pe,parse	; Z80 processor set the parity flag to signify overflow (8080 doesn't).
		ld	de,#err_msg	; Display error message.
		ld	c,#0x09		; Print string.
		jp	0x0005		; Jump to BDOS (when BDOS returns program will exit).
parse:
		ld	hl,#0x80	; Pointer to command line.
		ld	bc,(#0x80)	; Length of command line.
		xor	a
		ld	b,a
		add	hl,bc
		inc	hl		; Offset to end of command line.
		ld	(hl),#0x00	; Terminate commans line with a null.
		ld	bc,#0		; Number of arguments.

		ld	hl,#0x80	; Pointer to command line.
		ld	de,#args	; Pointer to ARGV.
		ld	(hl),a		; Add delimiter.
		jr	save
scan:
		inc	hl		; Move to next char in buffer
		ld	a,(hl)
		or	a		; Set flags
		jr	z,done
		cp	#' '
		jr	nz,scan
		xor	a
		ld	(hl),a		; Terminate parameter.
loop:
		inc	hl		; Move to next char in buffer
		ld	a,(hl)
		or	a		; Set flags
		jr	z,done
		cp	#' '
		jr	z,loop
save:
		ex	de,hl		; Store address of string in argv[].
		ld	(hl),e
		inc	hl		; Increment pointer to argv[]
		ld	(hl),d
		inc	hl		; Increment pointer to argv[]
		ex	de,hl
		inc	bc		; Increment number of arguments.
		ld	a,c
		cp	#35
		jr	z,done		; Stop parsing after 34 arguments!
		jr	scan

done:
		ld 	(stack),sp	; Save the stack pointer.
		ld	sp,#stack
		xor	a
		ld	h,a		; Number of arguments.
		ld	l,c
		ld	de,#args	; Pointer to arguments.
		call	_main		; Call the C main routine
		ld	sp,(stack)	; Restore original stack pointer
		ret

_bdos:
		ld	c,a		; DE already points to string!
		call	#5
		ex	de,hl		; Return result in DE
		ret
_putchar:
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
