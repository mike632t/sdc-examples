#
# makefile
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Note - Shell commands must start with a tab character at the beginning
# of each line NOT spaces..!
#
#  30 Jul 23   0.1   - Initial version - MT
#   4 Aug 23         - Added backup files to tar archive - MT
#
PROJECT	=  sdc-examples

EXCLUDE	=  sdc-cpm.c
SOURCE	=  $(filter-out $(EXCLUDE), $(wildcard *.c)) # Compile all source files 
OTHER		=  $(wildcard *.s)
SUPPORT	=  $(wildcard *.s) 
INCLUDE	=  $(wildcard *.h) 	# Automatically get all include files 
BACKUP	=  $(wildcard *.c.[0-9]) $(wildcard *.s.[0-9])
OBJECT	=  $(SOURCE:.c=.rel)
PROGRAM	=  $(SOURCE:.c=.com)

FILES	=  $(SOURCE) $(OTHER) $(EXCLUDE) $(BACKUP) $(INCLUDE) LICENSE README.md makefile .gitignore .gitattributes
LANG	=  LANG_$(shell (echo $$LANG | cut -f 1 -d '_'))
UNAME	=  $(shell uname)

LIBS	= 
FLAGS	=  -mz80 --data-loc 0 --no-std-crt0 

CC	= sdcc

make:$(PROGRAM) $(OBJECT)

all:clean $(PROGRAM) $(OBJECT)

# Compile sources
%.rel : %.c 
	@$(CC) $(FLAGS) -c  -o $@ $<

# Link
%.ihx: %.rel 
	@$(CC) $(FLAGS) -o $@ sdc-crt0-args.rel sdc-cpm.rel $< 

# Load
%.com: %.ihx 
	@sdobjcopy -Iihex -Obinary --gap-fill 0 $< $@ 
	@ls --color $@  

clean:
	@rm -f *.asm *.sym *.lst $(OBJECT)
	@rm -f *.map *.noi *.lk $(PROGRAM)
	
backup: clean
	@echo "$(PROJECT)-`date +'%Y%m%d%H%M'`.tar.gz"; tar -czpf ..\/$(PROJECT)-`date +'%Y%m%d%H%M'`.tar.gz $(FILES)
