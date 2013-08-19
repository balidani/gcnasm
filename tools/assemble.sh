#!/bin/bash

# Remove temporary files
rm -f _tmp*

# Pre-process input with cpp
cpp -E $3 > _tmp1.isa
tail -n +5 _tmp1.isa > _tmp2.isa

# Use gcnasm to assemble the input
run/gcnasm _tmp2.isa _tmp1.bin

# Patch the dummy ELF
python tools/dummy_elf_patcher/patch_dummy.py tools/dummy_elf_patcher/sample_dummy.bin _tmp1.bin _tmp2.bin

# Load the ELF into OpenCL
run/binary_gen $1 $2 _tmp2.bin -
