# File: Makefile for CSCI 485 Assignment 1.
# Tristan W. Bonsor 536 865 512

TITLE     = CSCI 485 Assignment 1: Rubik\'s Cube Solver
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
PROGRAM   = rube
GENERATOR = generator

INCLUDEDIR = include
SRCDIR     = src
LIBDIR     = libsearchai-1.0

GENDIR = gen

SRCEXT  = .cc
HEADEXT = .hpp
OBJEXT  = .o
DEPEXT  = .dep

SRC = rube.cc RubikColor.cc  RubikHeuristic.cc  RubikParser.cc  RubikScanner.cc

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
INCLUDES = -I./$(INCLUDEDIR) -I$(LIBDIR)/$(INCLUDEDIR)

# Memory test settings
MFLAGS = --tool=memcheck --leak-check=yes --show-reachable=yes #--num-callers=20 --track-fds=yes

# Build Rules
all: display-title $(PROGRAM) $(GENERATOR) clean

$(PROGRAM): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

%$(OBJEXT): %$(SRCEXT)
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: $(GENERATOR)
$(GENERATOR):
	@set -e; \
	$(CD) $(GENDIR); $(MK);
	@set -e; \
	$(CP) $(GENDIR)/$(GENERATOR) . ; \
	$(RM) $(GENDIR)/$(GENERATOR)

.PHONY: display-title
display-title:
	@set -e; \
	$(ECHO) $(TITLE) $(VERSION); \
	$(ECHO) $(COPYRIGHT)

.PHONY: mem-test
mem-test: all
	@set -e; \
	./$(GENERATOR) 3 7 > memtest.txt; \
	$(MEM) $(MFLAGS) ./$(PROGRAM) -v memtest.txt; \
	$(RM) memtest.txt

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