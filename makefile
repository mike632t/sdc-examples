#
# makefile
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty ofgcc-echo.
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
#  12 Sep 23         - Delete temporary files - MT
#  16 Sep 23         - Only include source files prefixed with 'sdc' - MT
#                    - Default runtime library defined a vairable - MT
#
PROJECT	=  sdc-examples

SOURCE	=  $(wildcard sdc-*.c) # Compile most source files
OTHER	=  $(wildcard sdc-*.s) 
INCLUDE	=  $(wildcard sdc-*.h) # Automatically get all include files 
BACKUP	=  $(wildcard sdc-*.c.[0-9]) $(wildcard sdc-*.s.[0-9])
OBJECT	=  $(SOURCE:.c=.rel)
PROGRAM	=  $(SOURCE:.c=.com)

FILES	=  $(SOURCE) $(OTHER) $(EXCLUDE) $(BACKUP) $(INCLUDE) LICENSE README.md makefile .gitignore .gitattributes
LANG	=  LANG_$(shell (echo $$LANG | cut -f 1 -d '_'))
UNAME	=  $(shell uname)

RUNTIME	=  sdc-crt0-args.rel
LIBS	= 
FLAGS	=  -mz80 --data-loc 0 --no-std-crt0

make:$(PROGRAM) $(OBJECT)

all:clean $(PROGRAM) $(OBJECT)

# Compile (and delete temporary files)
%.rel : %.c 
	@sdcc $(FLAGS) -c -o $@ $<
	@rm $(subst .c,.asm,$<)
	@rm $(subst .c,.sym,$<)
	@rm $(subst .c,.lst,$<)

# Link (and delete temporary files)
%.ihx: %.rel 
	@sdcc $(FLAGS) -o $@ $(RUNTIME) $< 
	@rm -f $(subst .rel,.map,$<)|| true
	@rm -f $(subst .rel,.noi,$<)|| true
	@rm -f $(subst .rel,.lk,$<)|| true
#	@rm -f $< || true # Don;t delete .rel files (forces rebuild).

# Load
%.com: %.ihx 
	@sdobjcopy -Iihex -Obinary --gap-fill 0 $< $@ 
	@rm -f $< || true 
	@ls $@

backup: clean
	@echo "$(PROJECT)-`date +'%Y%m%d%H%M'`.tar.gz"; tar -czpf ..\/$(PROJECT)-`date +'%Y%m%d%H%M'`.tar.gz $(FILES)	

clean:
	@rm -f $(OBJECT)|| true
	@rm -f $(PROGRAM)|| true
