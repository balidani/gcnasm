# AMD GCN ISA Assembler
#
# Dummy ELF patcher tool
# (Only for temporary use until we have something better)
#
# This software is Copyright 2013, Daniel Bali <balijanosdaniel at gmail.com>,
# and it is hereby released to the general public under the following terms:
# Redistribution and use in source and binary forms, with or without
# modification, are permitted.
 
import sys

def usage():
    print "Usage: %s <dummy.bin> <microcode.bin> <output.bin>" % sys.argv[0]

def main():

    if len(sys.argv) != 4:
        usage()
    
    with open(sys.argv[1], 'rb') as f1:
        with open(sys.argv[2], 'rb') as f2:
            
            data = f1.read()
            patch = f2.read()
            
            # Find the first GCN ISA instruction in the binary
            # Note: if there is a matching byte sequence earlier, 
            # this would cause an error
            offset = data.index('\x00\x03\x88\xc0')
            
            # Substitute data
            data_l = list(data)
            patch_l = list(patch)
            data_l[offset:offset+len(patch)] = patch
            
            # Save output
            with open(sys.argv[3], 'wb') as f3:
                f3.write(''.join(data_l))
    
if __name__ == "__main__":
    main()