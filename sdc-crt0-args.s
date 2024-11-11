;
;   crt0-args.s
;
;   Copyright(C) 2004   Konamiman (https://github.com/Konamiman/MSX)
;   Copyright(C) 2023   MT
;
;   Enhanced crt0.s for generic CP/M that copies the command line arguments
;   from the command tail into argv[] and passes argc and argv[] to main.
;
;   This allows main() to be defined as 'int main(int argc, char* argv[])'.
;
;   CP/M stores the length of the command line in memory at 0080H, followed
;   by the command line itself which may be up to 127 characters long.  The
;   code  used to parse the command line originally starts at 0100H, but is
;   relocated to C000H and the memory from 0100H is then used to store  the
;   arguments which are passed to main().
;
;   Although  modified for CP/M this routine _should_ still work on MSX-DOS
;   however, this hasn't been tested.  
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
;** 20 Aug 23   0.1     - Initial version - MT
;** 20 Aug 23   0.1	   - Initial version - MT 
;               0.2     - Order of the parameters passed to main() reversed
;                         to make it the same as other C compilers - MT
;
;   To Do:		         - Add a check for the CPU type.
;                       - Rewrite to use new calling convention.
;			               - The value in argv[0] should either be the program
;			                 name or an empty string.
;			               - Allow  command line parameters to be enclosed  in
;			                 quotes.
;                       - Relocate code to just below the BDOS.  
;
		.title	crt0-args.s
		.globl	_main

		.globl  l__INITIALIZER
		.globl  s__INITIALIZED
		.globl  s__INITIALIZER

		.area 	_HEADER (ABS)

		.org	0x0100			; Start address.

init:   	call	gsinit			; Initialize globals

;*  Check if there are any parameters

		ld	a,(#0x80)
		or	a
		ld	c,#0
		jr	z,cont

;*  Terminate command line with 0 

		ld	hl,#0x81
		ld	bc,(#0x80)
		ld	b,#0
		add	hl,bc
		ld	(hl),#0

;*  Copy  the  command line processing code to 0xC000 and execute  it  from
;   there, this way the memory of the original code can be recycled for the
;   parameter pointers table. 
;
;   The space from 0x100 up to 'cont' to gives room for about 40 parameters
;   more than enough for any real world application!

		ld	hl,#parloop
		ld	de,#0xC000
		ld	bc,#parloopend-#parloop
		ldir
		
;*  Initialize registers and jump to the loop routine
		
		ld	hl,#0x81		; Command line pointer
		ld	c,#0			; Number of params found
		ld	ix,#0x0100		; Params table pointer
		
		ld	de,#cont		; To continue execution at "cont"
		push	de			; when the routine RETs
		jp	0xC000
		
;*  Begin processing the command line

parloop:	ld	a,(hl)
		or	a			; Command line end found?
		ret	z

		cp	#32			; Skip spaces
		jr	nz,parfnd
		inc	hl
		jr	parloop

;*  Parameter found: add its address to params table...

parfnd:		ld	(ix),l
		ld	1(ix),h
		inc	ix
		inc	ix
		inc	c
		
		ld	a,c			; Protection against too many parameters
		cp	#40
		ret	nc

;*  ...and skip chars until finding a space or command line end.

parloop2:	ld	a,(hl)
		or	a			; Command line end found?
		ret	z

		cp	#32
		jr	nz,nospc		; If space found, set it to 0
						; (string terminator)...
		ld	(hl),#0
		inc	hl
		jr	parloop			; ...and return to space skipping loop

nospc:		inc	hl
		jr	parloop2

parloopend:
		
;*  Command line processing done. Here, C=number of parameters.

cont:		ld	hl,#0x0100
		ld	b,#0
		push	hl			; Pass info as parameters to "main"
		push	bc

;-- Step 3: Call the "main" function

		push	de
		ld	de,#_HEAP_start
		ld 	(_heap_top),de
		pop 	de

		call	_main

;-- Exit program when main is finished.

		pop	bc			; Unload the stack
		pop	hl
		ret				; and return.

;-- Alternativly perform a warm reset.

;		ld	c,#0			; Call BDOS RESET function
;		jp	5


;-- Place data after program code, and data init code after data

		.area	_CODE			; Program code area
		.area	_DATA			; Data area
_heap_top::	.dw 0				; Address of the start of the heap area

		.area   _GSINIT
gsinit::
		ld	bc,#l__INITIALIZER
		ld	a,b
		or	a,c
		jp	z,gsinext
		ld	de,#s__INITIALIZED
		ld	hl,#s__INITIALIZER
		ldir
gsinext:
		.area   _GSFINAL
		ret

;		.area	_OVERLAY		; These areas don't seem to be required.
;		.area	_HOME
;		.area	_BSS
		.area	_HEAP
_HEAP_start::					; Space for the heap.
