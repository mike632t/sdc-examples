#
# makefile
#
#  This  program is free software: you can redistribute it and/or modify it
#  under  the terms of the GNU General Public License as published  by  the
#  Free  Software Foundation, either version 3 of the License, or (at  your
#  option) any later version.
#
#  This  program  is distributed in the hope that it will  be  useful,  but
#  WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#  Public License for more details.
#
#  You  should have received a copy of the GNU General Public License along
#  with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Note - Shell commands must start with a tab character at the beginning
# of each line NOT spaces..!
#
#  30 Jul 23   0.1   - Initial version - MT
#   4 Aug 23         - Added backup files to tar archive - MT
#  12 Sep 23         - Delete temporary files - MT
#  16 Sep 23         - Only include source files prefixed with 'sdc' - MT
#  22 Sep 26         - Default runtime library defined in a variable - MT
#                    - Don't include sdc-cpm.c - MT
#  24 Sep 26         - Updates runtime as part of cleanup - MT
#
PROJECT	=  sdc-examples

SOURCE	=  $(filter-out sdc-cpm.c, $(wildcard sdc-*.c)) # Compile most source files
OTHER	=  $(wildcard sdc-*.s)
INCLUDE	=  $(wildcard sdc-*.h) # Automatically get all include files
BACKUP	=  $(wildcard sdc-*.c.[0-9]) $(wildcard sdc-*.s.[0-9])
OBJECT	=  $(SOURCE:.c=.rel)
PROGRAM	=  $(SOURCE:.c=.com)

FILES	=  $(SOURCE) sdc-cpm.c $(OTHER) $(BACKUP) $(INCLUDE) LICENSE README.md makefile .gitignore .gitattributes
LANG	=  LANG_$(shell (echo $$LANG | cut -f 1 -d '_'))
UNAME	=  $(shell uname)

RUNTIME	=  sdc-crt0
LIBS	=
CFLAGS	=  -mz80 --no-std-crt0 --data-loc 0
LDFLAGS	=  sdc-cpm.rel $(RUNTIME).rel

.SUFFIXES:  # Disable built-in suffix rules (required for older version of make)

make:$(PROGRAM) $(OBJECT)

all:clean $(PROGRAM) $(OBJECT)

# Compile the runtime code
runtime: $(RUNTIME).rel sdc-cpm.rel

$(RUNTIME).rel: $(RUNTIME).s
	@sdasz80 -o $@ $<

# Compile cpm support code
sdc-cpm.rel: sdc-cpm.c
	@sdcc -mz80 -c $<
	@rm -f sdc-cpm.asm sdc-cpm.lst sdc-cpm.sym

# Compile (and delete any working files)
%.rel : %.c
	@sdcc $(CFLAGS) -c -o $@ $<
	@rm -f $(subst .c,.asm,$<)
	@rm -f $(subst .c,.sym,$<)
	@rm -f $(subst .c,.lst,$<)

# Link (and delete working files)
%.ihx: %.rel $(RUNTIME).rel sdc-cpm.rel
	@sdcc $(CFLAGS) $(LDFLAGS) -o $@ $<
	@rm -f $(subst .rel,.map,$<)|| true
	@rm -f $(subst .rel,.noi,$<)|| true
	@rm -f $(subst .rel,.lk,$<)|| true
#	@rm -f $< || true # Don;t delete .rel files (forces rebuild).

# Load
%.com: %.ihx
	@sdobjcopy -Iihex -Obinary --gap-fill 0 $< $@
	@rm -f $< || true
	@ls --color $@
	
$(SOURCE:.c=): %: %.com

backup: clean
	@	@_date=`date +'%Y%m%d%H%M'`;_branch="`command -v git >/dev/null 2>&1 && git rev-parse --abbrev-ref HEAD 2>/dev/null || echo ""`"; \
	if [ -z "$$_branch" ]; then \
		archive="$(PROJECT)-$$_date.tar.gz"; \
	else \
		archive="$(PROJECT)-$$_branch-$$_date.tar.gz"; \
	fi; \
	tar -czpf "../$$archive" $(FILES) && cd .. && ls --color $$archive 2>/dev/null || ls ../$$archive 2>/dev/null || true ls --color "../$(PROJECT)-`date +'%Y%m%d%H%M'`.tar.gz"; 

clean:
	@rm -f $(OBJECT) $(PROGRAM) $(RUNTIME).rel sdc-cpm.rel
