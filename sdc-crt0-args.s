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
;** To Do	- Check number of arguments doesn't overflow tha array!
;
;
		.module	crt0

		.globl	_main

		.area	_HEADER (ABS)
		.org	0x0100

		ld	a,#0x7f		; Load with largest positive signed value.
		inc	a		; Incrementing should result in an overflow.
		jp	pe,parse	; Z80 processor set the parity flag to signify overflow (8080 doesn't).
		ld	de,#err_msg	; Display error message.
		ld	c,#0x09		; Print string.
		jp	0x0005		; Jump to BDOS (when BDOS returns program will exit).
err_msg:
		.str	"Z80 processor required."
		.db	00,13,10,'$'
args:
		.dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		.dw	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
		jr	scan

done:		xor	a
		ld	h,b		; Number of arguments.
		ld	l,c
		ld	de,#args	; Pointer to arguments.

		call	_main		; Jump to the C main routine
		ret

;		.area	_TPA		; Ordering of segments for the linker.
;		.area	_HOME
		.area	_CODE
		.area	_DATA
;
