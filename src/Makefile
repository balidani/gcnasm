CC = gcc
RM = rm -f

TD = cutest

CFLAGS = -Wall -Waggregate-return
MAIN = assembler.c
CFILES = error.c parser.c		\
	isa_instr.c isa_operand.c isa_format.c	\
	sop2.c sopk.c sop1.c sopc.c sopp.c 		\
	smrd.c vop2.c vop1.c vopc.c 

TESTFILES = $(TD)/unit_tests.c $(TD)/CuTest.c $(TD)/SOP2_test.c

ASSEMBLER = ../run/gcnasm
UNITTESTS = ../run/testit

default:
	$(CC) $(CFILES) $(MAIN) $(CFLAGS) -o $(ASSEMBLER)

tests:
	$(CC) $(CFILES) $(TESTFILES) -o $(UNITTESTS)

clean:
	$(RM) $(ASSEMBLER) $(UNITTESTS)
