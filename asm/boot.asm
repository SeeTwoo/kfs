[bits 32]
section .multiboot
    align 4
    dd 0x1BADB002
    dd 0x00
    dd -(0x1BADB002 + 0x00)

section .text
global _start
extern kmain

_start:
    cli
    lgdt [gdt_descriptor]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.reload_cs

.reload_cs:
    mov esp, stack_top
    call kmain
    hlt

section .bss
resb 8192
stack_top:

section .data
    align 4
gdt_start:
    ; 1. Null descriptor
    dd 0x0
    dd 0x0

    ; 2. Code segment descriptor (offset 0x08)
    dw 0xFFFF       ; limit (0-15)
    dw 0x0000       ; base (0-15)
    db 0x00         ; base (16-23)
    db 10011010b    ; access byte (exec/read)
    db 11001111b    ; flags + limit (16-19)
    db 0x00         ; base (24-31)

    ; 3. Segment descriptor (offset 0x10)
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size (16 bits)
    dd gdt_start                ; Address (32 bits)
