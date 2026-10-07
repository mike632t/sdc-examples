/*
 * sdc-bdos.h - interface to CP/M BDOS functions.
 *
 * Copyright(C) 2023   MT
 *
 * Provides support for the CP/M operating system.
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
 * 20 Aug 23         - Initial version - MT
 * 26 Sep 26         - Removed function prototypes - MT
 * 07 Oct 26         - Modified and extended defined values - MT
 * 
 * To Do             - 
 *
 */

/* ASCII control characters */

#define  Asc$NUL           0x00                 /* <Ctrl/@> - Null. */
#define  Asc$SOH           0x01                 /* <Ctrl/A> */
#define  Asc$STX           0x02                 /* <Ctrl/B> */
#define  Asc$BRK           0x03                 /* <Ctrl/C> - CP/M break in character. */
#define  Asc$EOT           0x04                 /* <Ctrl/D> */
#define  Asc$ENQ           0x05                 /* <Ctrl/E> */
#define  Asc$ACK           0x06                 /* <Ctrl/F> */
#define  Asc$BEL           0x07                 /* <Ctrl/G> - ASCII bell etc... */
#define  Asc$BS            0x08                 /* <Ctrl/H> - Backspace. */
#define  Asc$TAB           0x09                 /* <Ctrl/I> - Tab. */
#define  Asc$LF            0x0A                 /* <Ctrl/J> - Linefeed. */
#define  Asc$VT            0x0B                 /* <Ctrl/K> - Vertical tab. */
#define  Asc$FF            0x0C                 /* <Ctrl/L> - Formfeed. */
#define  Asc$CR            0x0D                 /* <Ctrl/M> - Carridge return. */
#define  Asc$SO            0x0E                 /* <Ctrl/N> */
#define  Asc$SI            0x0F                 /* <Ctrl/O> */
#define  Asc$DLE           0x10                 /* <Ctrl/P> */
#define  Asc$DC1           0x11                 /* <Ctrl/Q> - XON. */
#define  Asc$DC2           0x12                 /* <Ctrl/R> */
#define  Asc$DC3           0x13                 /* <Ctrl/S> - XOFF. */
#define  Asc$DC4           0x14                 /* <Ctrl/T> */
#define  Asc$NAK           0x15                 /* <Ctrl/U> */
#define  Asc$SYN           0x16                 /* <Ctrl/V> */
#define  Asc$ETB           0x17                 /* <Ctrl/W> */
#define  Asc$CAN           0x18                 /* <Ctrl/X> */
#define  Asc$EOM           0x19                 /* <Ctrl/Y> */
#define  Asc$EOF           0x1A                 /* <Ctrl/Z> */
#define  Asc$ESC           0x1B                 /* <Ctrl/[> */
#define  Asc$FS            0x1C                 /* <Ctrl/\> - File seperator. */
#define  Asc$GS            0x1D                 /* <Ctrl/]> - Group seperator. */
#define  Asc$RS            0x1E                 /* <Ctrl/^> - Record seperator. */
#define  Asc$US            0x1F                 /* <Ctrl/-> - Unit seperator. */
                         
#define  Asc$SP            0x20                 /* Space. */
#define  Asc$DEL           0x7F                 /* Delete. */
#define  Asc$AST           0x2A                 /* Asterisk. */
#define  Asc$SLH           0x2F                 /* Slash. */
#define  Asc$PT            0x2E                 /* Full stop. */
#define  Asc$SEP           0x2C                 /* Seperator (comma). */
#define  Asc$ZERO          0x30
#define  Asc$NINE          0x39
#define  Asc$EQU           0x3D
#define  Asc$PLUS          0x2B
#define  Asc$MINUS         0x2D

/* Important CP/M system constants. */

#define  CPM$Base          0x0000               /* Base of CP/M OS. */
#define  CPM$Buffer        0x0080               /* Start of CP/M file buffer 80..FFH. */
#define  CPM$Tail          0x0080               /* Command tail. */
#define  CPM$FCB1          0x005C               /* Default FCB, file operand 1. */
#define  CPM$FCB2          0x006C */
#define  CPM$Boot          0x0000               /* BIOS reboot. */
#define  CPM$Load          0x0100               /* Program load address. */
#define  CPM$BDOS          0x0005

/* BIOS and BDOS function codes. */

#define  BDOS$Reset        0x00                 /* System reset (Warmboot). */
#define  BDOS$CON_Input    0x01                 /* Input byte from CON:. */
#define  BDOS$CON_Output   0x02                 /* Type one byte at CON:. */
#define  BDOS$RDR_Input    0x03                 /* Input one byte from RDR:. */
#define  BDOS$PUN_Output   0x04                 /* Output one byte to PUN:. */
#define  BDOS$LST_Output   0x05                 /* Output one byte to LST:. */
#define  BDOS$Direct_IO    0x06                 /* Direct console I/O. */
#define  BDOS$Get_IOByte   0x07                 /* Get IOBYTE. */
#define  BDOS$Set_IOByte   0x08                 /* Set IOBYTE. */
#define  BDOS$Print_Str    0x09                 /* Print a string to CON:. */
#define  BDOS$Read_Str     0x0A                 /* Bufferted read from CON:. */
#define  BDOS$CON_Status   0x0B                 /* Get console status. */
#define  BDOS$Ver_No       0x0C                 /* Return version number. */
#define  BDOS$Reset_Disk   0x0D                 /* Reset disks and login to A:. */
#define  BDOS$Select       0x0E                 /* Select default drive. */
#define  BDOS$Open_File    0x0F                 /* Open a disk file. */
#define  BDOS$Close_File   0x10                 /* Close output file. */
#define  BDOS$Find_File    0x11                 /* Initial directory search. */
#define  BDOS$Find_Next    0x12                 /* Sucessive directory searches. */
#define  BDOS$Erase_File   0x13                 /* Erase a file. */
#define  BDOS$Read_Seq     0x14                 /* Read disk sequential. */
#define  BDOS$Write_Seq    0x15                 /* Write disk sequential. */
#define  BDOS$Make_File    0x16                 /* Make a directory entry. */
#define  BDOS$Ren_File     0x17                 /* Rename a file. */
#define  BDOS$Get_Drive    0x19                 /* Get default drive number. */
#define  BDOS$Set_DMA      0x1A                 /* Set file buffer address. */
#define  BDOS$Get_Vector   0x1B                 /* Get address of allocation vector. */
#define  BDOS$Get_DPB      0x1F                 /* Get address of Disk Parameter Block. */
#define  BDOS$Set_Usr_No   0x20                 /* Get/Set default user number. */
#define  BDOS$Read_Rnd     0x21                 /* Read disk random. */
#define  BDOS$Write_Rnd    0x22                 /* Write disk random. */

#define  BIOS$Boot         0x00                 /* Cold bootstrap. */
#define  BIOS$Warmboot     0x03                 /* Warm bootstrap (restart). */
#define  BIOS$CON_Stat     0x06                 /* Console input status. */
#define  BIOS$CON_Input    0x09                 /* Console input. */
#define  BIOS$CON_Output   0x0C                 /* Console output. */
#define  BIOS$LST_Output   0x0F                 /* List output. */
#define  BIOS$PUN_Output   0x12                 /* Punch output. */
#define  BIOS$RDR_Input    0x15                 /* Reader input. */
#define  BIOS$Home         0x18                 /* Home disk heads (set to track 0). */
#define  BIOS$Select       0x1B                 /* Select disk (no error trap). */
#define  BIOS$Set_Track    0x1E                 /* Set track number. */
#define  BIOS$Set_Sector   0x21                 /* Set sector number. */
#define  BIOS$Set_DMA      0x24                 /* Set DMA address. */
#define  BIOS$Read         0x27                 /* Read sector. */
#define  BIOS$Write        0x2A                 /* Write sector. */
#define  BIOS$LST_Stat     0x2D                 /* List output status. */
