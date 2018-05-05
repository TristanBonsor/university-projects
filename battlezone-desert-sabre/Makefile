TITLE     = BattleZone: Desert Sabre
VERSION   = v1.0
COPYRIGHT = Copyright \(C\) 2014 Tristan W. Bonsor

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
PROGRAM  = sabre

INCLUDEDIR = include
SRCDIR     = src
LIBDIR     =

SRCEXT  = .cc
HEADEXT = .hpp
OBJEXT  = .o
DEPEXT  = .dep

SRC = Camera.cc Controller.cc Main.cc TankControlComponent.cc

vpath %$(SRCEXT) $(SRCDIR)
vpath %$(HEADEXT) $(INCLUDEDIR)
vpath %$(DEPEXT) $(INCLUDEDIR)

OBJECTS = $(SRC:$(SRCEXT)=$(OBJEXT))
DEPENDS = $(SRC:$(SRCEXT)=$(DEPEXT))

# Compiler Settings
CC = g++
WARNINGS = -W -Wall -Wextra
CFLAGS = -g -O0 $(WARNINGS) -std=c++11
LDFLAGS = -lGL -lGLU -lglut
INCLUDES = -I./$(INCLUDEDIR)

# Memory test settings (Valgrind)
MFLAGS = --tool=memcheck --leak-check=yes --show-reachable=yes
PROGRAMARGS = 

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
	$(MEM) $(MFLAGS) ./$(PROGRAM) $(PROGRAMARGS) -v memtest.txt; \
	$(RM) memtest.txt

# Build Clean-up
.PHONY: clean
clean:
	$(RM) -f *$(OBJEXT) *$(DEPEXT)

.PHONY: dist-clean
dist-clean: clean
	$(RM) -f $(PROGRAM)

# Dependency Tracking
-include $(DEPENDS)

%$(DEPEXT): %$(SRCEXT)
	@set -e; \
	$(RM) -f $@; \
	$(CC) -MM -MP $(CFLAGS) $(INCLUDES) -c $< > $@.$$$$; \
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	$(RM) -f $@.$$$$