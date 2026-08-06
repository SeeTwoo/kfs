target remote :1234
symbol-file awix.elf
set architecture i386
b kmain
set pagination off
