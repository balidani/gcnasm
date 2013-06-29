CC = gcc
RM = rm -f

CFLAGS = -Wall
CFILES = assembler.c error.c parser.c		\
	isa_instr.c isa_operand.c isa_format.c	\
	sop2.c sop1.c vop2.c


ASSEMBLER = ../run/assembler

default:
	$(CC) $(CFILES) $(CFLAGS) -o $(ASSEMBLER)

clean:
	$(RM) $(ASSEMBLER)