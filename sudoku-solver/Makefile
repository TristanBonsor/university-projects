# Makefile for CSCI 485 Lab 7.

TITLE     = CSCI 485 Lab 7: Sudoku Constraint Satisfaction Problem
VERSION   = v1.0
COPYRIGHT = Copyright \(C\)  2013 Tristan W. Bonsor

# Utilities
CD   = cd
CP   = cp
ECHO = echo
MEM  = valgrind
MK   = make
MV   = mv
RM   = rm
SED  = sed

# Build Settings
PROGRAM   = sudoku

INCLUDEDIR = include
SRCDIR     = src

SRCEXT  = .cc
HEADEXT = .hpp
OBJEXT  = .o
DEPEXT  = .dep

SRC = main.cc

vpath %$(SRCEXT) $(SRCDIR)
vpath %$(HEADEXT) $(INCLUDEDIR)
vpath %$(DEPEXT) $(INCLUDEDIR)

OBJECTS = $(SRC:$(SRCEXT)=$(OBJEXT))
DEPENDS = $(SRC:$(SRCEXT)=$(DEPEXT))

# Compiler Settings
CC = g++
WARNINGS = -W -Wall -Wextra
CFLAGS = -g $(WARNINGS) -std=c++11
LDFLAGS =
INCLUDES = -I./$(INCLUDEDIR)

# Memory test settings
MFLAGS = --tool=memcheck --leak-check=yes --show-reachable=yes #--num-callers=20 --track-fds=yes

# Build Rules
all: display-title $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

%$(OBJEXT): %$(SRCEXT)
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: display-title
display-title:
	@set -e; \
	$(ECHO) $(TITLE) $(VERSION); \
	$(ECHO) $(COPYRIGHT)

.PHONY: mem-test
mem-test: all
	@set -e; \
	$(MEM) $(MFLAGS) ./$(PROGRAM) -v 

# Build Clean-up
.PHONY: clean
clean:
	$(RM) -f *$(OBJEXT) *$(DEPEXT)

.PHONY: dist-clean
dist-clean: clean
	$(RM) -f $(PROGRAM) $(GENERATOR)

# Dependency Tracking
-include $(DEPENDS)

%$(DEPEXT): %$(SRCEXT)
	@set -e; \
	$(RM) -f $@; \
	$(CC) -MM -MP $(CFLAGS) $(INCLUDES) -c $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$