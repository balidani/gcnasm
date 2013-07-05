CC = gcc
RM = rm -f
VAL = valgrind

CFLAGS = -Wall -Wextra -Wconversion -Wundef -Wshadow 				\
	-Waggregate-return -Wunreachable-code

CFILES = assembler.c error.c parser.c isa_instr.c isa_operand.c 	\
	isa_format.c sop2.c sopk.c sop1.c sopc.c sopp.c smrd.c vop2.c 	\
	vop1.c vopc.c mubuf.c mtbuf.c mimg.c


GCNASM = ../run/gcnasm
TEST = ../test

default:
	$(CC) $(CFILES) $(CFLAGS) -o $(GCNASM)

clean:
	$(RM) $(GCNASM)

memtest:
	$(VAL) --tool=memcheck --leak-check=full $(GCNASM) 				\
		$(TEST)/basic_test.isa output.bin
	$(RM) output.bin