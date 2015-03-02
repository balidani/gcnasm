AMD GCN ISA assembler
=====================

GCN ISA assembler tool for my GSoC project at Openwall

Project page: http://openwall.info/wiki/john/development/GCN-ISA
GitHub: https://github.com/balidani/gcnasm

### Usage

    ./gcnasm <input> <output>
    
The input is a text file containing the assembly code that is to be processed. The output will be a binary file containing the microcode.

### Notes

* s_endpgm needs a parameter to work (e.g. s_endpgm 0)
* some macros for s_waitcnt need to be defined (e.g. lgkmcnt(3))
      #define lgkmcnt(3) 0x037f
* for some instruction types only one format is used (VOPC is always used as VOP3b)

### Other projects

Ryan White created a GCN Assembler in C#: 
* http://www.codeproject.com/Articles/872477/Assembler-for-AMD-s-GCN-GPU
