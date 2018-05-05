# File: Makefile for CSCI 485 Assignment 1: Rubik's cube state generator.
# Copyright (C) 2013 Tristan W. Bonsor

TITLE = CSCI 485 Assignment 1: Rubik\'s Cube Generator
VERSION = v1.0
COPYRIGHT = Copyright \(C\)  2013 Tristan W. Bonsor

# Utilities
CD = cd
CP = cp
ECHO = echo
MEM = valgrind
MK = make
MV = mv
RM = rm
SED = sed

# Build Settings
PROGRAM = generator

INCLUDEDIR = include
SRCDIR = ../src src
LIBDIR = libsearchai-1.0

SRCEXT = .cc
HEADEXT = .hpp
OBJEXT = .o
DEPEXT = .dep

SRC = generator.cc RubikColor.cc  RubikHeuristic.cc  RubikParser.cc  RubikScanner.cc

vpath %$(SRCEXT) $(SRCDIR)
vpath %$(HEADEXT) $(INCLUDEDIR)
vpath %$(DEPEXT) $(INCLUDEDIR)

OBJECTS = $(SRC:$(SRCEXT)=$(OBJEXT))
DEPENDS = $(SRC:$(SRCEXT)=$(DEPEXT))

# Compiler Settings
CC = g++
WARNINGS = -W -Wall -Wextra
CFLAGS = -O2 $(WARNINGS) -std=c++11
LDFLAGS =
INCLUDES = -I../$(INCLUDEDIR) -I../$(LIBDIR)/$(INCLUDEDIR)

# Memory test settings
MFLAGS = --tool=memcheck --leak-check=yes --show-reachable=yes #--num-callers=20 --track-fds=yes

# Build Rules
all: display-title $(PROGRAM) clean

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
mem-test: $(PROGRAM)
	$(MEM) $(MFLAGS) ./$<

# Build Clean-up
.PHONY: clean
clean:
	$(RM) -f *$(OBJEXT) *$(DEPEXT)

.PHONY: dist-clean
dist-clean: clean
	$(RM) -f $(PROGRAM) $(LIB)

# Dependency Tracking
-include $(DEPENDS)

%$(DEPEXT): %$(SRCEXT)
	@set -e; \
	$(RM) -f $@; \
	$(CC) -MM -MP $(CFLAGS) $(INCLUDES) -c $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$