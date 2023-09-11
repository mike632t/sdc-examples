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
 * 
 * ToDo
 *
 */

#define S_RESET		0
#define C_READ       1
#define C_WRITE      2
#define A_READ       3
#define A_WRITE      4
#define L_WRITE      5
#define C_RAWIO      6
#define GETIOBYTE    7
#define SETIOBYTE    8
#define C_WRITESTR   9
#define C_READSTR    10
#define DRV_ALLRESET 13
#define DRV_SET      14
#define F_OPEN       15
#define F_CLOSE      16
#define F_SMATCH     17
#define F_NMATCH     18
#define F_DELETE     19
#define F_READ       20
#define F_WRITE      21
#define F_MAKE       22
#define F_RENAME     23
#define DRV_LOGINVEC 24
#define DRV_GET      25
#define F_DMAOFF     26
#define DRV_ALLOCVEC 27
#define DRV_SETRO    28
#define DRV_ROVEC    29
#define F_ATTRIB     30
#define DRV_DPB      31
#define F_USERNUM    32
#define F_READRAND   33
#define F_WRITERAND  34
#define F_SIZE       35
#define F_RANDREC    36
#define DRV_RESET    37
#define F_WRITEZF    40

unsigned int bdoscall(unsigned char C_reg, unsigned int DE_reg) __naked;

int putchar(int c);
