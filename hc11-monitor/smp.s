;===============================================================================
; File:    
        .file "smp.s"
; Copyright (C) 2012, 2013  Tristan W. Bonsor
;
; This work is free software; you can redistribute it and/or modify it under
; the terms of the GNU General Public License as published by the Free
; Software Foundation, version 2.
	
; Behaviour: Simple monitor program for the Axiom HC11 board. Allows user to
;            assemble code and data into memory, dump memory, and change
;            program execution.
;
; Assumptions: Must be boot strapped via the buffalo monitor. Designed to be
;              used in tandem with buffalo monitor I/O routines. When code and
;              data is assembled into a memory location an opcode for rts
;              (return from subroutine) is written to the highest adjacent
;              memory location. Commands are one character in length and a
;              user's input is limited to a string 20 characters in length.
;              Limited error checking is performed on input.
;===============================================================================
.sect .text
.global _start

_start:
	lds	#_stack	; Initialize the stack pointer.
	ldx	#TITLE	; Load start address of title string into IX.
	jsr	printS	; Output title string.

mainlp:
	jsr	printLF	; Output CR+LF.
	ldx	#PROMPT	; Load start address of prompt string into IX.
	jsr	printS	; Output prompt string.

	ldx	#BUFFER	; Load start address of input string buffer into IX.
	jsr	getS	; Get string from input and store in string buffer.

	jsr	seekWC	; Move to first non-whitespace char of input string.
	jsr	isEOT	; Test if input string is some form of blank line.
	beq	mainlp	; If input is blank ignore and prompt for new input.

	ldy	#CMD	; Otherwise, load address of command vector into IY.
	jsr	cpWrd	; Copy command from input string to vector.

	;; HACK: The next 5 lines test if input command is at least 1 character
	;; 	 longer than command vector capacity or we are at end of
	;; 	 input string.
	jsr	isEOT	; Test if end of input string.
	beq	params	; If so load up parameter vectors with 'EOT'.
	ldaa	0,x	; Otherwise, load character after copied word into A.
	jsr	isWSpc	; Test if character value in A is whitespace.
	bne	cmdErr	; Command invalid (more than one character).

params:
	;; Get command parameters if any.
	jsr	seekWC	; Otherwise, seek out parameters (if exists).
	ldy	#PR1	; Load address of parameter 1 vector into IY.
	jsr	cpWrd	; Copy parameter into parameter 1 vector.
	jsr	seekWC	; Seek out last parameter (if it exists).
	ldy	#PR2	; Load address of parameter 2 vector into IY.
	jsr	cpWrd	; Copy parameter into parameter 2 vector.

	;; Sanitize command and parameters.
	ldx	#CMD	; Load address of command vector into IX.
	jsr	ucWrd	; Convert all lower case characters in vector to upper.
	ldx	#PR1	; Load address of parameter 1 vector into IX.
	jsr	ucWrd	; Convert all lower case characters in vector to upper.
	ldx	#PR2	; Load address of parameter 2 vector into IX.
	jsr	ucWrd	; Convert all lower case characters in vector to upper.

	;; Process command. Could be parcelled off to subroutines in future.
	ldx	#CMD+1	; Hack-a-doodle-doo.
	ldaa	0,x	; Load value of command into A.
	psha		; Save value in A on stack.

	ldab	#0x41	; Load value for ascii character 'A'.
	sba		; Subtract value in B from value in A.
	beq	asm	; Command is 'A'.

	pula		; Otherwise, restore value in A.
	psha		; Save value in A on stack.
	ldab	#0x44	; Load value for ascii character 'D'.
	sba		; Subtract value in B from value in A.
	beq	dmp	; Command is 'D'.

	pula		; Otherwise, restore value in A.
	psha		; Save value in A on stack.
	ldab	#0x47	; Load value for ascii character 'G'.
	sba		; Subtract value in B from value in A.
	beq	run	; Command is 'G'.

	pula		; Otherwise, restore value in A.
	psha		; Save value in A on stack.
	ldab	#0x48	; Load value for ascii character 'H'.
	sba		; Subtract value in B from value in A.
	beq	menu	; Command is 'H'.
	
	pula		; Otherwise, restore value in A.
	psha		; Save value in A on stack.
	ldab	#0x51	; Load value for ascii character 'Q'.
	sba		; Subtract value in B from value in A.
	beq	exit	; Command is 'Q' proceed to program exit.

	bra	cmdErr	; Invalid command.

asm:
	ldx	#PR1	; Load parameter 1 vector.
	jsr	hex2bit	; Convert hex parameter to its bit pattern.
	bne	cmdErr	; Error in parameter.
	
	xgdy		; Load address specified by parameter 1 into IY.

	ldx	#PR2	; Load parameter 2 vector.
	jsr	hex2bit	; Convert hex parameter to its bit pattern.
 	bne	cmdErr	; Error in parameter.
	
	staa	0,y	; Store bit pattern in A into location via IY.
	ldaa	#0x39	; Load rts opcode into A.
	staa	1,y	; Hack-a-doodle-doo once again.
	bra	valid	; Command completed.

dmp:
	ldx	#PR2	; Load parameter 2 vector.
	jsr	hex2bit	; Convert hex parameter to its bit pattern.
	bne	cmdErr	; Error in parameter.

	psha
	
	ldx	#PR1	; Load parameter 1 vector.
	jsr	hex2bit	; Convert hex parameter to its bit pattern.
	bne	cmdErr	; Error in parameter.

	xgdx		; Load address specified by parameter 1 into IX.

	pula
	inca

nwline:
	ldab	#0x08	; 8 entries per line.

smline:
	;; Hack to get around buffalo I/O trashing A without telling us.
	psha
	jsr	printBS	; Output contents at memory address and increment address.
	pula

	deca		; Decrement number of remaining addresses.
	beq	valid	; Will output cc+1 times.

	decb		; Keep track of number of entries per line.
	bne	smline	; Line not full yet.
	
	jsr	printLF	; Otherwise line is full, output CR+LF.
	bra	nwline	; Repeat output procedure on new line.

run:
	ldx	#PR1	; Load parameter 1 vector.
	jsr	hex2bit	; Convert hex parameter to its bit pattern.
	bne	cmdErr	; Error in parameter.
	
	xgdx		; Load address specified by parameter 1 into IX.

	jsr	0,x	; Execute code at address specified by IX.
	bra	valid	; If the code is ok our "hidden" rts will get us home.

menu:
	ldx	#MENUA	;
	jsr	printS	;
	ldx	#MENUD	;
	jsr	printS	;
	ldx	#MENUG	;
	jsr	printS	;
	ldx	#MENUH	;
	jsr	printS	;
	ldx	#MENUQ	;
	jsr	printS	;

	bra	mainlp	;

valid:	
	bra	mainlp	; Command was valid, get next input.

cmdErr:
	ldx	#INVALID	; Load start address of error string.
	jsr	printS		; Display error message.
	bra	mainlp

exit:
	ldx	#EXIT		; Load start address of exit string.
	jsr	printS		; Display exit string.

	swi

;-------------------------------------------------------------------------------
; Behaviour: Gets string input terminated by a CR or LF, whichever is
;            encountered first (both will be discarded). Input string terminated
;            by EOT.
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;              Utilizes INCHAR buffalo monitor I/O routine.
;              Value stored in IX is beginning of contiguous storage space
;              Hard coded string length limit = 20 characters.
;
; Restriction: Subroutine may only access registers, the stack, and the INCHAR
;              subroutine.
;
; Register and Memory Usage:
;
;  Precondition: IX contains address of first storage space for first character
;                of input string.
;
;  Postcondition: IX contains address of first character of input string.
;
;  Destroys: None.
;-------------------------------------------------------------------------------
getS:
	tpa		; Save value in CCR on stack.
	pshx		; Save value in IX on stack.
	pshy		; Save value in IY on stack.
	psha		; Save value in A on stack.
	pshb		; Save value in B on stack.
	
	ldy	#0x0015	; Load the input limit+1 counter into IY.

gslp:
	jsr	getC	; Block until character input in A.
	psha		; Save input character value on stack.

	ldab	#0x0D	; Load CR value into B.
	sba		; Subtract value in B from value in A.
	beq	eoi	; If 0 is result, input character was a CR.

	pula		; Otherwise restore input character value into A.
	psha		; Save input character value on stack.
	ldab	#0x0A	; Load LF value into B.
	sba		; Subtract value in B from value in A.
	beq  	eoi	; If 0 is result, input character was a LF.

	pula		; Otherwise restore input character value into A.
	staa	0,x	; Store input character value in buffer.
	dey		; Decrement the input counter.
	beq	tmnt	; If input limit reached proceed to termination.
	
	inx		; Otherwise increment to next location in buffer.
	bra	gslp	; Get another character.

eoi:
	ins		; Ignore input character value on stack.

tmnt:	
	ldaa	#0x04	; Load EOT value into A.
	staa	0,x	; Store EOT into buffer (signals end of input string).

	pulb		; Restore value in B.
	pula		; Restore value in A.
	puly		; Restore value in IY.
	pulx		; Restore value in IX.
	tap		; Restore value in CCR.

xgetS:
	rts

;-------------------------------------------------------------------------------
; Behaviour: Copies the characters in a string up to the first whitespace
;            character into a vector. If a whitespace character is encountered
;            prior to filling vector, ascii value for '\0' will end the valid
;            contents of the vector. Similar behaviour if 'EOT' is encountered.
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;              Utilizes buffalo monitor I/O routines.
;              Value stored in IX is a location in contiguous storage space.
;              String will be terminated by ascii EOT.
;              The vector passed to this function will have its data capacity
;              as a value in the first element.
;
; Restriction: Subroutine may only access registers, the stack, and the various
;              buffalo monitor I/O subroutines.
;
; Register and Memory Usage:
;
;  Precondition: IX contains address of a location in contiguous storage space
;                (address of character in string).
;                IY contains start address of vector.
;
;  Postcondition: IX contains address of first whitespace character following
;                 the copied word or character with value 'EOT'.
;                 IY will contain the address of the vector that contains the
;                 copied word.
;
;  Destroys: None.
;-------------------------------------------------------------------------------
cpWrd:
	tpa		; Save value in CCR on stack.
	pshy		; Save value in IY on stack.
	psha		; Save value in A on stack.
	pshb		; Save value in B on stack.

	ldab	0,y	; Load storage capacity of vector into B.
	beq	cpWCl	; If capacity is 0 proceed to clean up.

cpWlp:	
	iny		; Increment vector.

	ldaa	0,x	; Load character value via IX into A.
	jsr	isWSpc	; Test if character is whitespace.
	beq	cpWws	; If whitespace proceed to insert '\0' into vector.

	staa	0,y	; Store character into vector from A.
	jsr	isEOT	; Test if value of character just copied is ascii 'EOT'.
	beq	cpWCl	; If character value is 'EOT' proceed to clean up.

	inx		; Otherwise get next character location in string.
	decb		; Keep track of remaining space in vector.
	bne	cpWlp	; If space remaining continue copying.
	bra	cpWCl	; Otherwise proceed to clean up.

cpWws:
	ldaa	#0x00	; Load ascii value for '\0' into A.
	staa	0,y	; Store '\0' into vector (signals end of valid contents).
cpWCl:
	pulb		; Restore value in B.
	pula		; Restore value in A.
	puly		; Restore value in IY.
	tap		; Restore value in CCR.

xcpWrd:
	rts

;-------------------------------------------------------------------------------
; Behaviour: Converts all lower case characters found in vector to upper case.
;            Converts up to end of vector or if ascii 'EOT' is found
; 
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;              Utilizes buffalo monitor I/O routines.
;              Value stored in IX is a location in contiguous storage space.
;              The vector passed to this function will have its data capacity
;              as a value in the first element.
;
; Restriction: Subroutine may only access registers, the stack, and the various
;              buffalo monitor I/O subroutines.
;
; Register and Memory Usage:
;
;  Precondition: IX contains start address of vector.
;
;  Postcondition: IX contains start address of vector.
;
;  Destroys: None.
;-------------------------------------------------------------------------------
ucWrd:
	tpa		; Save value in CCR on stack.
	pshx		; Save value in IX on stack.
	psha		; Save value in A on stack.
	pshb		; Save value in B on stack.

	ldab	0,x	; Load storage capacity of vector into B.
	beq	ucWCl	; If capacity is 0 proceed to clean up.

ucWlp:
	inx		; Increment vector.

	jsr	isEOT	; Test if character value is ascii 'EOT'.
	beq	ucWCl	; Character is 'EOT', proceed to clean up.

	ldaa	0,x	; Otherwise, load character value via IX into A.
	jsr	isLCas	; Test if value in A is in ascii lower case range.
	bne	ucWnxt	; If not lower case get next character.

	jsr	uCase	; Otherwise convert to upper case.

ucWnxt:
	decb		; Keep track of remaining space in vector.
	staa	0,x	; Store new value in vector.
	bne	ucWlp	; If space remaining continue converting.

ucWCl:
	pulb		; Restore value in B.
	pula		; Restore value in A.
	pulx		; Restore value in IX.
	tap		; Restore value in CCR.

xucWrd:
	rts

;-------------------------------------------------------------------------------
; Behaviour: Converts hex value in vector into its 16 or 8 bit value.
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;              Value stored in IX is a location in contiguous storage space.
;              Vector contains a 4 digit or 2 digit hex number.
;              The vector passed to this function will have its data capacity
;              as a value in the first element.
;
; Restriction: Subroutine may only access registers, the stack, and the WCHEK
;              subroutine.
;
; Register and Memory Usage:
;
;  Precondition: IX contains start address of a vector.
;
;  Postcondition: IX contains start address of vector.
;                 A contains 8 bit value or D contains 16 bit value.
;                 Z bit is set to 1 if valid input, 0 otherwise.
;
;  Destroys: None.
;-------------------------------------------------------------------------------
hex2bit:
	pshx		; Save value in IX on stack.
	pshy		; Save value in IY on stack.

	clra
	clrb

	ldab	0,x	; Load capacity of vector into IY.
	xgdy
	beq	h2bErr	; If capacity is 0 proceed to error.

	inx		; Otherwise, proceed to first digit.

	ldaa	0,x	; Load first digit into A.

	jsr	isB16	; Test if digit is valid hex.
	bne	h2bErr	; Not a valid hex digit, proceed to cleanup with error.

	jsr	isB10	; Test if digit is '0' to '9'.
 	bne	AtoF	; Digit is A-F.

	ldab	#0x30	; Load value of '0' into B.
	sba		; Subtract value in B from value in A.
	bra	fxdg1	; Proceed to fix first digit.

AtoF:
	ldab	#0x37	; Load value of '7' into B.
	sba		; Subtract value in B from value in A.

fxdg1:
	dey		; Keep track of valid space in vector.
	beq	h2bErr	; Not a 8 or 16 bit hex number.

	rola		; Shift bits in A left 4 times
	rola
	rola
	rola
	psha		; Store bit value of first digit on stack.
	
	inx		; Get address of next digit.

	ldaa	0,x	; Load second digit into A.
	
	jsr	isB16	; Test if digit is valid hex.
	bne	h2bErrr	; Not a valid hex digit, proceed to cleanup with error.

	jsr	isB10	; Test if digit is '0' to '9'.
	bne	AtoF2	; Digit is A-F.

	ldab	#0x30	; Load value of '0' into B.
	sba		; Subtract value in B from value in A.
	bra	fxdg2	; Proceed to fix second digit.

AtoF2:	
	ldab	#0x37	; Load value of '7' into B.
	sba		; Subtract value in B from value in A.

fxdg2:
	pulb		; Store bit pattern to first digit in B.
	aba		; Add value in B to value in A (first 2 digits).

 	dey		; Keep track of valid space in vector.
	beq	h2bCl	; Valid 8 bit number, proceed to clean up.
	
	psha		; Otherwise save value in A on stack.

	inx		; Get address of next digit.

	ldaa	0,x	; Load third digit into A.
	jsr	isB16	; Test if digit is valid hex.
	bne	h2bErrr	; Not a valid hex digit, proceed to cleanup with error.

	jsr	isB10	; Test if digit is '0' to '9'.
	bne	AtoF3	; Digit is A-F.

	ldab	#0x30	; Load value of '0' into B.
	sba		; Subtract value in B from value in A.
	bra	fxdg3	; Proceed to fix first digit.
	
AtoF3:
	ldab	#0x37	; Load value of '7' into B.
	sba		; Subtract value in B from value in A.

fxdg3:
	dey		; Keep track of valid space in vector.
	beq	h2bErrr	; Not a 16 bit hex number.

	rola		; Shift bits in A left 4 times
	rola
	rola
	rola
	psha		; Store bit value of third digit on stack.

	inx 		; Get address of next digit.

	ldaa	0,x	; Load fourth digit into A.
	jsr	isB16	; Test if digit is valid hex.
	bne	h2bErrrr; Not a valid hex digit, proceed to cleanup with error.

	jsr	isB10	; Test if digit is '0' to '9'.
	bne	AtoF4	; Digit is A-F.

	ldab	#0x30	; Load value of '0' into B.
	sba		; Subtract value in B from value in A.
	bra	fxdg4	; Proceed to fix fourth digit.

AtoF4:
	ldab	#0x37	; Load value of '7' into B.
	sba		; Subtract value in B from value in A.

fxdg4:
	pulb		; Store bit pattern to third digit in B.
	aba		; Add value in B to value in A (last 2 digits).

	psha		; Get bit pattern in proper order.
	pulb		;
	pula		;

	ldy	#0x0000	; Set Z bit to 1 (good operation).
	bra	h2bCl	; Proceed to clean up.

h2bErrrr:
	pula		; Clean stack.

h2bErrr:
	pula		; Clean stack.

h2bErr:
	iny		; Set Z bit to 0 (error).

h2bCl:
	puly		; Restore value in IY.
	pulx		; Restore value in IX.

xhex2bit:
	rts

;-------------------------------------------------------------------------------
; Behaviour: Seeks past whitespace in a string, stopping at the first
;            non-whitespace character.
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;              Utilizes WCHEK buffalo monitor I/O routine.
;              Value stored in IX is a location in contiguous storage space.
;              String will be terminated by ascii EOT.
;
; Restriction: Subroutine may only access registers, the stack, and the WCHEK
;              subroutine.
;
; Register and Memory Usage:
;
;  Precondition: IX contains address of a location in contiguous storage space
;                (address of character in string).
;
;  Postcondition: IX contains address of first non-whitespace character found
;		  from start of seek.
;
;  Destroys: None.
;-------------------------------------------------------------------------------
seekWC:
	tpa		; Save value in CCR on stack;
	psha		; Save value in A on stack.

skwclp:
	ldaa	0,x	; Load character value via IX into A.
	jsr	isWSpc	; Test if character in A is whitespace.
	bne	wcfnd	; If non-whitespace character found.
	
	inx		; Otherwise get address of next character into IX.
	bra	skwclp	; Repeat test for whitespace.

wcfnd:
	pula		; Restore value in A.
	tap		; Restore value in CCR.

xseekWC:
	rts

;-------------------------------------------------------------------------------
; Behaviour: Verifies if character is a base 10 digit.
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;
; Restriction: Subroutine may only access registers, the stack, and the various
;              buffalo I/O subroutines.
;
; Register and Memory Usage:
;
;  Precondition: A contains ascii value of potential base 10 digit.
;
;  Postcondition: A contains value of potential base 10 digit.
;                 Sets Z bit to 1 if valid digit, 0 otherwise.
;
;  Destroys: None.
;-------------------------------------------------------------------------------
isB10:
	pshb		; Save value in B on stack.
	psha		; Save value in A on stack

	ldab #0x30	; Load value of '0' into B.
	sba		; Subtract value in B from value in A.
	blt  b10Cl	; Value in A is not a base 10 digit.

	pula		; Restore value in A.
	psha		; Save value in A on stack.
	ldab #0x39	; Load value of '9' into B.
	sba		; Subtract value in B from value in A.
	bgt  b10Cl	; Value in A is not a base to digit.

	ldaa #0x39	; Load value of '9' into A.
	sba		; Set Z bit to 1 (valid digit).

b10Cl:
	pula		; Restore value in A.
	pulb		; Restore value in B.

xisB10:
	rts

;-------------------------------------------------------------------------------
; Behaviour: Verifies if character is a base 16 (hex) digit.
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;              Digits a-f will be upper case (A-F).
;
; Restriction: Subroutine may only access registers, the stack, and the various
;              buffalo I/O subroutines.
;
; Register and Memory Usage:
;
;  Precondition: A contains ascii value of potential hex digit.
;
;  Postcondition: A contains value of potential hex digit.
;                 Sets Z bit to 1 if valid digit, 0 otherwise.
;
;  Destroys: None.
;-------------------------------------------------------------------------------
isB16:
	pshb		; Save value in B on stack.
	psha		; Save value in A on stack.

	jsr	isB10	; Test if digit is '0' through '9'.
	beq	b16Cl	; Is valid digit.

	ldab	#0x41	; Otherwise, load value of 'A' into B.
	sba		; Subtract the value in B from the value in A.
	blt	b16Cl	; Not a valid hex digit.

	pula		; Restore value in A.
	psha		; Save value in A on stack.
	ldab	#0x46	; Save value of 'F' into B.
	sba
	bgt	b16Cl	; Not a valid hex digit.

	ldaa	#0x46	; Load A with same value in B.
	sba		; Set Z bit to 1 (is a valid hex digit).

b16Cl:
	pula		; Restore value in A.
	pulb		; Restore value in B.

xisB16:
	rts

;-------------------------------------------------------------------------------
; Behaviour: Tests if a character value is ascii 'EOT'.
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;              Value stored in IX is a location where the character to be
;              tested resides.
;
; Restriction: Subroutine may only access registers and the stack.
;
; Register and Memory Usage:
;
;  Precondition: IX contains address of a location in which an ascii character
;                resides.
;
;  Postcondition: Z bit will be 1 if character is 'EOT', 0 otherwise.
;
;  Destroys: None.
;-------------------------------------------------------------------------------
isEOT:
	psha		; Save value in A on stack.
	pshb		; Save value in B on stack.

	ldaa	0,x	; Load value found via IX into A.
	ldab	#0x04	; Load ascii value for 'EOT' into B.
	sba		; Subtract value in B from value in A (set Z bit).

	pulb		; Restore value in B.
	pula		; Restore value in A.

xisEOT:
	rts
;-------------------------------------------------------------------------------
; Behaviour:   Test for lower case character.
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;
; Restriction: Subroutine may only access registers and the stack.
;
; Register and Memory Usage:
;
;  Precondition:  A contains value of ascii character.
;
;  Postcondition: Z bit set to 1 if character is lower case, 0 otherwise.
;
;  Destroys:      None.
;-------------------------------------------------------------------------------
isLCas:
	pshb		; Save value in B on stack.
	psha		; Save character value in A on stack.

	ldab	#0x61	; Load value of 'a' into B.
	sba		; Subtract value in B from value in A.
	blt	nLCas	; If value in A < 0 character is not lower case.

	pula		; Otherwise restore input character value in A.
	psha		; Save character value in A on stack.
	ldab 	#0x7A	; Load value of 'z' into B.
	sba		; Subtract value in B from value in A.
	bgt	nLCas	; If value in A > 0 character is not lower case.

	pshb		; Push value in B onto stack.
	pula		; Pull value on stack into A.
	sba		; Subtract value in B from value in A (set Z bit to 1).
	bra	LCasCl	; Proceed to clean up.

nLCas:
	incb		; Increment value in B (set Z bit to 0).

LCasCl:
	pula		; Restore input character in A.
	pulb		; Restore value in B.
	
xisLCas:
	rts

;-------------------------------------------------------------------------------
; Behaviour:   Test for upper case character.
;
; Assumptions: Designed to be used in tandom with buffalo monitor I/O routines.
;
; Restriction: Subroutine may only access registers and the stack.
;
; Register and Memory Usage:
;
;  Precondition:  A contains ascii value of character.
;
;  Postcondition: Z bit set to 1 if character is upper case, 0 otherwise.
;
;  Destroys:      None.
;-------------------------------------------------------------------------------
isUCas:
	pshb		; Save value in B on stack.
	psha		; Save character value in A on stack.

	ldab	#0x41	; Load value of 'A' into B.
	sba		; Subtract value in B from value in A.
	blt	nUCas	; If value in A < 0 character is not upper case.

	pula		; Otherwise restore input character value in A.
	psha		; Save character value in A on stack.
	ldab	#0x5A	; Load value of 'Z' into B.
	sba		; Subtract value in B from value in A.
	bgt	nUCas	; If value in A > 0 character is not upper case.

	pshb		; Push value in B onto stack.
	pula		; Pull value on stack into A.
	sba		; Subtract value in B from value in A (set Z flag to 1).
	bra	UCasCl	; Proceed to clean up.

nUCas:
	incb		; Increment value in B (set Z flag to 0).
	
UCasCl:
	pula		; Restore input character in A.
	pulb		; Restore value in B.

xisUCas:
	rts

;-------------------------------------------------------------------------------
; Buffalo I/O subroutine wrappers.
;-------------------------------------------------------------------------------

; UPCASE
uCase:	jsr 0xFFA0
xuCase:	rts

; WCHEK
isWSpc:  jsr 0xFFA3
xisWSpc: rts

; DCHEK
isDelim:  jsr 0xFFA6
xisDelim: rts

; OUT1BSP
printBS:  jsr 0xFFBE
xprintBS: rts

; OUTCRLF
printLF:  jsr 0xFFC4
xprintLF: rts

; OUTSTRG
printS:  jsr 0xFFC7
xprintS: rts

; INCHAR
getC:	jsr 0xFFCD
xgetC:	rts

;-------------------------------------------------------------------------------
; Data Section
;-------------------------------------------------------------------------------

; Input string buffer.
BUFFER:	.asciz ""
.space 21, 1

; Output strings.
TITLE: .asciz "Simple Monitor Program\04"
.space 21, 1
MENUA: .asciz "a bbbb cc - Assemble byte 0xcc into location 0xbbbb\04"
.space 64, 1
MENUD: .asciz "d bbbb cc - Dump contents in 0xbbbb to 0xbbbb+cc\04"
.space 64, 1
MENUG: .asciz "g bbbb    - Go to 0xbbbb\04"
.space 32, 1
MENUH: .asciz "h         - Display this menu\04"
.space 48, 1
MENUQ: .asciz "q         - Return control to buffalo monitor\04"
.space 64, 1
PROMPT:	.asciz "$> \04"
.space 21, 1
INVALID: .asciz "Invalid command\04"
.space 21, 1
EXIT: .asciz "Program exit...\04"
.space 21, 1

; Vectors (Convention: first element stores value of maximum capacity).
CMD: .byte 1,0		; Stores command.
PR1: .byte 4,0,0,0,0	; Stores parameter 1.
PR2: .byte 2,0,0	; Stores parameter 2.

.end