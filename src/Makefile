CC = gcc
RM = rm -f

CFLAGS = -Wall
CFILES = assembler.c error.c parser.c		\
	isa_instr.c isa_operand.c isa_format.c	\
	sop2.c sop1.c sopc.c vop2.c vop1.c vopc.c 


ASSEMBLER = ../run/gcnasm

default:
	$(CC) $(CFILES) $(CFLAGS) -o $(ASSEMBLER)

clean:
	$(RM) $(ASSEMBLER)
