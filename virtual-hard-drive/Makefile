################################################################################
# File:   Makefile
# Author: Tristan W. Bonsor
################################################################################

PRGM=vdManager

VPATH=src

SED=sed

INCLUDEDIR=
LIBDIR=
LIB=

SRCEXT=.c
SRC=main.c vhd.c
OBJ=$(SRC:$(SRCEXT)=.o)
DEP=$(SRC:$(SRCEXT)=.dep)

CC=gcc
CFLAGS=-W -Wall -Wextra -pedantic -ansi -g
LDFLAGS=

$(PRGM): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

%.o: %$(SRCEXT)
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(OBJ) $(PRGM) $(SAMPLE)

include: $(DEP)

%.dep: %$(SRCEXT)
	@set -e; \
	$(RM) $@; \
	$(CC) -MM $(CFLAGS) -c $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$