target remote :1234
symbol-file awix.elf
set architecture i386
b kmain
b schedule
set pagination off
