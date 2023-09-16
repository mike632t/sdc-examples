;
;-- crt0.s
;
;   Modified runtime for generic CP/M.
;
;   sdasz80 -o sdc-crt0.s
;
;   This program is free software: you can redistribute it and/or modify it
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
;** 16 Sep 23	- Implemented getchar() using direct BIOS calls and changed
;                 putchar() to call the BIOS - MT
;               - Reformatted the code and modified all the private  labels
;                 to use upper and lower case (it is just a style thing but
;                 I  think it makes it more obvious which functions  can be
;                 called from C) - MT
;
		.module	crt0
;
		.globl	_main
;
		.area	_HEADER (ABS)
;
;-- ASCII control characters.
;
Asc$NUL		.equ	0x00			; <Ctrl/@> - Null.
Asc$SOH		.equ	0x01			; <Ctrl/A>
Asc$STX		.equ	0x02			; <Ctrl/B>
Asc$BRK		.equ	0x03			; <Ctrl/C> - CP/M break in character.
Asc$EOT		.equ	0x04			; <Ctrl/D>
Asc$ENQ		.equ	0x05			; <Ctrl/E>
Asc$ACK		.equ	0x06			; <Ctrl/F>
Asc$BEL		.equ	0x07			; <Ctrl/G> - ASCII bell etc...
Asc$BS		.equ	0x08			; <Ctrl/H> - Backspace.
Asc$TAB		.equ	0x09			; <Ctrl/I> - Tab.
Asc$LF		.equ	0x0A			; <Ctrl/J> - Linefeed.
Asc$VT		.equ	0x0B			; <Ctrl/K> - Vertical tab.
Asc$FF		.equ	0x0C			; <Ctrl/L> - Formfeed.
Asc$CR		.equ	0x0D			; <Ctrl/M> - Carridge return.
Asc$SO		.equ	0x0E			; <Ctrl/N>
Asc$SI		.equ	0x0F			; <Ctrl/O>
Asc$DLE		.equ	0x10			; <Ctrl/P>
Asc$DC1		.equ	0x11			; <Ctrl/Q> - XON.
Asc$DC2		.equ	0x12			; <Ctrl/R>
Asc$DC3		.equ	0x13			; <Ctrl/S> - XOFF.
Asc$DC4		.equ	0x14			; <Ctrl/T>
Asc$NAK		.equ	0x15			; <Ctrl/U>
Asc$SYN		.equ	0x16			; <Ctrl/V>
Asc$ETB		.equ	0x17			; <Ctrl/W>
Asc$CAN		.equ	0x18			; <Ctrl/X>
Asc$EOM		.equ	0x19			; <Ctrl/Y>
Asc$EOF		.equ	0x1A			; <Ctrl/Z>
Asc$ESC		.equ	0x1B			; <Ctrl/[>
Asc$FS		.equ	0x1C			; <Ctrl/\> - File seperator.
Asc$GS		.equ	0x1D			; <Ctrl/]> - Group seperator.
Asc$RS		.equ	0x1E			; <Ctrl/^> - Record seperator.
Asc$US		.equ	0x1F			; <Ctrl/-> - Unit seperator.
;
Asc$SP		.equ	0x20			; Space.
Asc$DEL		.equ	0x7F			; Delete.
Asc$AST		.equ	0x2A			; Asterisk.
Asc$SLH		.equ	0x2F			; Slash.
Asc$PT		.equ	0x2E			; Full stop.
Asc$SEP		.equ	0x2C			; Seperator (comma).
Asc$ZERO	.equ	0x30
Asc$NINE	.equ	0x39
Asc$EQU		.equ	0x3D
Asc$PLUS	.equ	0x2B
Asc$MINUS	.equ	0x2D
;
;-- Important CP/M system constants.
;
CPM$Base	.equ    0x0000			; Base of CP/M OS.
CPM$Buff	.equ	0x0080			; Start of CP/M file buffer 80..FFH.
CPM$Tail	.equ	0x0080			; Command tail.
CPM$FCB1	.equ	0x005C			; Default FCB, file operand 1.
CPM$FCB2	.equ	0x006C
CPM$Boot	.equ	0x0000			; BIOS reboot.
CPM$Load	.equ	0x0100			; Program load address.
CPM$BDOS	.equ	0x0005
;
;-- BIOS and BDOS function codes.
;
;   Provides a tidy means of accessing both BDOS and BIOS routines directly
;   from  the users program. As such it is system dependant since the  page
;   will  vary from system to system. It may be easily found as the MSB  of
;   the address specified in the JMP instruction at 0000H. 
;
BDOS$Reset	.equ	0x00			; System reset (Warmboot).
BDOS$CON_Input	.equ	0x01			; Input byte from CON:.
BDOS$CON_Output	.equ	0x02			; Type one byte at CON:.
BDOS$RDR_Input	.equ	0x03			; Input one byte from RDR:.
BDOS$PUN_Output	.equ	0x04			; Output one byte to PUN:.
BDOS$LST_Output	.equ	0x05			; Output one byte to LST:.
BDOS$Direct_IO	.equ	0x06			; Direct console I/O.
BDOS$Get_IOByte	.equ	0x07			; Get IOBYTE.
BDOS$Set_IOByte	.equ	0x08			; Set IOBYTE.
BDOS$Print_Str	.equ	0x09			; Print a string to CON:.
BDOS$Read_Str	.equ	0x0A			; Bufferted read from CON:.
BDOS$CON_Status	.equ	0x0B			; Get console status.
BDOS$Ver_No	.equ	0x0C			; Return version number.
BDOS$Reset_Disk	.equ	0x0D			; Reset disks and login to A:.
BDOS$Select	.equ	0x0E			; Select default drive.
BDOS$Open_File	.equ	0x0F			; Open a disk file.
BDOS$Close_File	.equ	0x10			; Close output file.
BDOS$Find_File	.equ	0x11			; Initial directory search.
BDOS$Find_Next	.equ	0x12			; Sucessive directory searches.
BDOS$Erase_File	.equ	0x13			; Erase a file.
BDOS$Read_Seq	.equ	0x14			; Read disk sequential.
BDOS$Write_Seq	.equ	0x15			; Write disk sequential.
BDOS$Make_File	.equ	0x16			; Make a directory entry.
BDOS$Ren_File	.equ	0x17			; Rename a file.
BDOS$Get_Drive	.equ	0x19			; Get default drive number.
BDOS$Set_DMA	.equ	0x1A			; Set file buffer address.
BDOS$Get_Vector	.equ	0x1B			; Get address of allocation vector.
BDOS$Get_DPB	.equ	0x1F			; Get address of Disk Parameter Block.
BDOS$Set_Usr_No	.equ	0x20			; Get/Set default user number.
BDOS$Read_Rnd	.equ	0x21			; Read disk random.
BDOS$Write_Rnd	.equ	0x22			; Write disk random.
;
;-- Symbol definations for BIOS service request numbers.
;
BIOS$Boot	.equ	0x00			; Cold bootstrap.
BIOS$Warmboot	.equ	0x03			; Warm bootstrap (restart).
BIOS$CON_Stat	.equ	0x06			; Console input status.
BIOS$CON_Input	.equ	0x09			; Console input.
BIOS$CON_Output	.equ	0x0C			; Console output.
BIOS$LST_Output	.equ	0x0F			; List output.
BIOS$PUN_Output	.equ	0x12			; Punch output.
BIOS$RDR_Input	.equ	0x15			; Reader input.
BIOS$Home	.equ	0x18			; Home disk heads (set to track 0).
BIOS$Select	.equ	0x1B			; Select disk (no error trap).
BIOS$Set_Track	.equ	0x1E			; Set track number.
BIOS$Set_Sector	.equ	0x21			; Set sector number.
BIOS$Set_DMA	.equ	0x24			; Set DMA address.
BIOS$Read	.equ	0x27			; Read sector.
BIOS$Write	.equ	0x2A			; Write sector.
BIOS$LST_Stat	.equ	0x2D			; List output status.
;
		.org	CPM$Load
;
		jp	Start			; Overwriting the initial code with succesive pointers
						; to each command line parameter allows up to 34 arguments
						; be passed to the main program.
Err_msg:	.str	"Z80 processor required."
		.db	00,13,10,'$'
;
Start:		ld	a,#0x7f			; Load with largest positive signed value.
		inc	a			; Incrementing should result in an overflow.
		jp	pe,Init			; Z80 processor set the parity flag to signify overflow (8080 doesn't).
		ld	de,#Err_msg		; Display error message.
		ld	c,#BDOS$Print_Str	; Print string.
		jp	BDOS			; Jump to BDOS (when BDOS returns program will exit).
;
Init:		ld 	(Stack),sp		; Save the stack pointer.
		ld	sp,#Stack
		call	_main			; Call main().
		ld	sp,(Stack)		; Restore original stack pointer.
		ret
;
BIOS:		ld	hl,(CPM$Base+1)		; Get address of warmstart routine and use hi byte as BIOS address.
		ld	l,c			; Put code (actually the offset into the jump table) in to LSB of address, and the BIOS 
		ld	c,e			; offset into the MSB. Put or word byte arguments into BC where the BIOS expects them.
		ld	b,d
		jp	(hl)			; Jump directly to BIOS function...
;
BDOS:		call	#CPM$BDOS		; Call BDOS
		ex	de,hl			; Return result in DE
		ret
;
_getchar::	ld	c,#BIOS$CON_Input	; Get character using a direct BIOS call.
		call	BIOS
		ld	e,a			; Return value in DE (it is an int not a char).
		xor	a
		ld	d,a
		ret	
;
_putchar::	ex	de,hl
		ld	a,e
		sub	a,#0x0a
		or	a,d
		jr	nz,NoLF
		push	de
		ld	de,#0x000d		; Print a carriadge return before any linefeed.
		ld	c,#BIOS$CON_Output
		call	BIOS
		pop	de
NoLF:		ld	c,#BIOS$CON_Output
		call	BIOS
		ld	de,#0x0000		; Return 0.
		ret
;
;		.area	_TPA			; Ordering of segments for the linker.
;		.area	_HOME
		.area	_CODE
		.area	_DATA
		.ds	256
Stack:		.dw	0
;
