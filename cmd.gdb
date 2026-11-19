target remote :1234
symbol-file awix.elf
set architecture i386
b kmain
b schedule
b ring_push
b ring_pop
set pagination off
