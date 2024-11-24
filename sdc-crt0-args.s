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
;** 22 Nov 24   0.1	- Initial version - MT 
;			- Added check CPU type - MT
;			- Arguments are now now ANSI compliant (returning a 
;                         null string for arg[0] is valid) - MT
;
;   To Do:		- Allow  command line parameters to be enclosed  in
;			  quotes.
;
			.title	crt0-args.s
;
			.globl	_main
			.globl	_bdos
;
			.area 	_HEADER (ABS)
;
			.org	0x0100			; Start address.
;
;-- Check CPU type (as sdcc requires a Z80)
;
start:			ld	a,#0x7f			; Load with largest positive signed value.
			inc	a			; Incrementing should result in an overflow.
			jp	pe,init			; Z80 processor set the parity flag to signify overflow (8080 doesn't).
			ld	de,#err_msg		; Display error message.
			ld	c,#0x09			; Print string.
			jp	0x0005			; Jump to BDOS (when BDOS returns program will exit).
err_msg:
			.str	"Z80 processor required."
			.db	13,10,'$'
;
;-- Check if there are any parameters
;
init:			ld	a,(#0x80)
			or	a
			ld	c,#0
			jr	z,cont
;
;-- Terminate command line with an ASCII NUL this will tell us when to stop 
;   scanning and terminate the last argument.
;
			ld	hl,#0x81
			ld	bc,(#0x80)
			ld	b,#0
			add	hl,bc
			ld	(hl),#0
;			
;-- Initialize registers and jump to the loop routine.
;			
			xor     a
			ld	c,a		; Clear number of parameters.
			ld      hl,#0x80	; Address of the command tail.
			ld      (hl),a		; Make argv[0] a nul string.

			ld      ix,#0x0100	; Over writes the CPU type checking.
;
;-- Add argv[0] add its address to array.
;			
			ld	(ix),l
			ld	1(ix),h
			inc	ix
			inc	ix
			inc	c
			ld      hl,#0x81	; Address of command tail
			call	parser
;			
;-- Command line processing done.
;
cont:			ld	hl,#0x0100	
			ld	b,#0		; C contains the number of arguments.
			push	hl		; Pass info as parameters to "main"
			push	bc
;
;-- Call the "main" function
;
			push	de
			ld	de,#_HEAP_start	; Save the address of the heap 
			ld 	(_heap_top),de
			pop 	de

			call	_main
;
;-- Exit program when main is finished.
;
			pop	bc		; Unload the stack
			pop	hl
			ret			; and return.
;
;-- Alternatively perform a warm reset.
;
;			ld	c,#0		; Call BDOS RESET function
;			jp	5
;			
;-- Begin processing the command line
;
parser:			ld	a,(hl)
			or	a		; Command line end found?
			ret	z

			cp	#32		; Skip spaces
			jr	nz,next
			inc	hl
			jr	parser
;
;-- Found another argument so add its address to array.
;
next:			ld	(ix),l		; Copy address to location pointed to by ix.
			ld	1(ix),h
			inc	ix		; Increment the pointer.
			inc	ix
			inc	c		; Increment the counter.
			
			ld	a,c		; Check the number of parameters
			cp	#10		; Max 16 parameters... 
			ret	nc		; Give up if there are too many.
;
;-- ...and skip chars until you find a space or the end of the command line.
;
loop:			ld	a,(hl)
			or	a		; Command line end found?
			ret	z

			cp	#32
			jr	nz,nospc
			ld	(hl),#0		; If space found, set it to zero (string terminator)...
			inc	hl
			jr	parser		; ...and start again.

nospc:			inc	hl
			jr	loop
;
;-- Place data after program code, and data init code after data

			.area	_CODE		; Program code area
			.area	_DATA		; Data area
_heap_top::		.dw 0			; Address of the start of the heap area

;			.area	_OVERLAY	; These areas don't seem to be required.
;			.area	_HOME
;			.area	_BSS
			.area	_HEAP
_HEAP_start::					; Space for the heap.
