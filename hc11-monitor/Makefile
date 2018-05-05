# Makefile for Simple Monitor Program

# Executable file name
EXE=smp

# All source file name(s) (assumed extension of .s)
SRC=smp

# All generated object file name(s)
OBJ=smp.o

# Tools 
AS=m68hc11-as
LD=m68hc11-ld
GDB=m68hc11-gdb
OC=m68hc11-objcopy

interactive: $(EXE).elf
	$(GDB) $(EXE).elf

clean:
	rm -f *.o *.elf *.s19 *.gdb *.lst $(EXE)


$(EXE).elf: impMod 
	$(LD)  -O0 --verbose -N -mshort -mm68hc11elfb $(OBJ) -o $(EXE).elf
	$(OC) -O srec --only-section=.text --only-section=.data $(EXE).elf $(EXE).s19

impMod:
	if (test "$(SRC)" != "none") then \
		for m in $(SRC); do \
			($(AS)   -al=$$m.lst --gstabs -o $$m.o  $$m.s) \
		done \
	fi
