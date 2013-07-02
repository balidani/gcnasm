#!/bin/bash

rm -f _tmp*

run/assembler $3 _tmp1.bin
python tools/dummy_elf_patcher/patch_dummy.py tools/dummy_elf_patcher/sample_dummy.bin _tmp1.bin _tmp2.bin
run/binary_gen $1 $2 _tmp2.bin -
