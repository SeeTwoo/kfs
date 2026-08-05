; io.asm
[bits 32]
section .text

global outb
global outw
global inb

; void outb(uint16_t port, u8 val)   ; cdecl: port=[esp+4], val=[esp+8]
outb:
    mov edx, [esp + 4]
    mov al,  [esp + 8]
    out dx, al
    ret

outw:
	mov edx, [esp + 4]
	mov eax, [esp + 8]
	out dx, ax
	ret

; u8 inb(uint16_t port)              ; cdecl: port=[esp+4]
inb:
    mov edx, [esp + 4]
    xor eax, eax
    in al, dx
    ret
