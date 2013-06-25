CC = gcc
RM = rm -f

CFLAGS = -Wall
CFILES = assembler.c isa_instr.c parser.c

ASSEMBLER = ../run/assembler

default:
	$(CC) $(CFILES) $(CFLAGS) -o $(ASSEMBLER)

clean:
	$(RM) $(ASSEMBLER)