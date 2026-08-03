target remote :1234
symbol-file kernel.bin
set architecture i386
b kmain
