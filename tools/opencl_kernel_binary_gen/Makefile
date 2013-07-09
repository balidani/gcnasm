# Basic "command" variables
CC = gcc
RM = rm -f

# Setup OCLROOT
ifdef NVIDIA_CUDA
OCLROOT = $(NVIDIA_CUDA)
else
OCLROOT = /usr/local/cuda
endif

ifdef AMDAPPSDKROOT
AMDAPP = -DAMDAPPSDK
OCLROOT = $(AMDAPPSDKROOT)
endif

ifdef ATISTREAMSDKROOT
AMDAPP = -DAMDAPPSDK
OCLROOT = $(ATISTREAMSDKROOT)
endif

# gcc options
CFLAGS = -Wall -I'$(OCLROOT)/include'
CFILES = binary_gen.c
        
# Executable path
BINARY_GEN = ../../run/binary_gen

default:
	@echo "To build, type:"
	@echo "x86-64	Linux, 64bit"
	@echo "x86	Linux, 32bit"

x86-64:
	$(CC) $(CFILES) $(CFLAGS) -L'$(OCLROOT)/lib/x86_64' -lOpenCL -o $(BINARY_GEN)

x86:
	$(CC) $(CFILES) $(CFLAGS) -L'$(OCLROOT)/lib/x86' -lOpenCL -o $(BINARY_GEN)	

clean:
	$(RM) $(BINARY_GEN)