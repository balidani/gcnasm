CC = gcc
RM = rm -f

CFLAGS = -Wall -Wextra -Wconversion -Wundef -Wshadow 				\
	-Waggregate-return -Wunreachable-code

CFILES = assembler.c error.c parser.c isa_instr.c isa_operand.c 	\
	isa_format.c sop2.c sopk.c sop1.c sopc.c sopp.c smrd.c vop2.c 	\
	vop1.c vopc.c mubuf.c


ASSEMBLER = ../run/gcnasm

default:
	$(CC) $(CFILES) $(CFLAGS) -o $(ASSEMBLER)

clean:
	$(RM) $(ASSEMBLER)
