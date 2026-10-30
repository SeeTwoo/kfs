[bits 32]
section .multiboot
    align 4
    dd 0x1BADB002
    dd 0x00
    dd -(0x1BADB002 + 0x00)

section .text
global _start
global stack_top
global stack_bottom
extern kmain

_start:
    cli

	mov esi, gdt_start
	mov edi, 0x800
	mov ecx, gdt_end - gdt_start
	cld
	rep movsb

    lgdt [gdt_descriptor]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	mov ax, 0x18
    mov ss, ax
    jmp 0x08:.reload_cs

.reload_cs:
    mov esp, stack_top
    call kmain
    hlt

section .bss
align 16
stack_bottom:
	resb 8192
stack_top:

section .gdt
    align 4
gdt_start:
    ; 1. Null descriptor
    dd 0x0
    dd 0x0

    ; 2. Kernel Mode Code segment descriptor (offset 0x08)
    dw 0xFFFF       ; limit (0-15)
    dw 0x0000       ; base (0-15)
    db 0x00         ; base (16-23)
    db 10011010b    ; access byte (exec/read)  / 0x9A -> 154
    db 11001111b    ; flags + limit (16-19)
    db 0x00         ; base (24-31)

    ; 3. Kernel Mode Data Segment descriptor (offset 0x10)
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b	; 0x92 -> 146
    db 11001111b
    db 0x00

	; 4. Kernel Stack
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b	; 0x92 -> 146
    db 11001111b
    db 0x00


    ; 5. User Mode Code Segment descriptor (offset 0x18)
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 11111010b	; 0xFA -> 250
    db 11001111b
    db 0x00

    ; 6. User Mode Data Segment descriptor (offset 0x20)
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 11110010b	; 0xF2 -> 242
    db 11001111b
    db 0x00

	; 7. User Stack
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 11110010b	; 0xF2 -> 242
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size (16 bits)
    dd 0x800                ; Address (32 bits)
