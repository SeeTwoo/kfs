global switch_to

switch_to:
	; prototype :
	; void	switch_to(u32 *prev_esp, u32 next_esp)

	; save EFLAGS including the if interrupt flag
	pushfd

	; save task A registers
	push ebp
	push ebx
	push esi
	push edi

	; save task A stack pointer
	mov eax, [esp + 24]
	mov [eax], esp

	; jump to task B
	mov edx, [esp + 28]
	mov esp, edx

	; restore taks B registers
	pop edi
	pop esi
	pop ebx
	pop ebp

	; restore EFLAGS including the if interrupt flag
	popfd

	ret
