; isr.asm
[bits 32]
section .text

extern c_handler ; routeur for all isr

; --- helper : charge l'IDT dans le CPU ---
global load_idt
load_idt:
	mov eax, [esp + 4]   ; eax = adresse de l'IDTR (argument cdecl)
	lidt [eax]           ; le CPU lit maintenant la table
	ret

; --- point d'entrée de l'IRQ clavier (vecteur 0x21) ---
global isr21
isr21:
	pusha                ; sauvegarde les 8 registres généraux (32 o)
	push 0x21            ; argument n° de vecteur pour c_handler
	call c_handler
	add esp, 4           ; dépile l'argument
	popa                 ; restaure les registres
	iret                 ; dépile EIP, CS, EFLAGS (poussés par le CPU)


;;Quand on aura besoin de declarer bcp d'isr
;
;%macro ISR_NOERR 1
;global isr%1
;isr%1:
;	push 0          ; faux error code pour uniformiser
;	push %1         ; le numéro
;	jmp isr_common_stub
;%endmacro
;
;%rep 256
;	ISR_NOERR i
;%endrep
