[bits 32]
[section .text]
global PT
global ReloadLDT
global LoadCr3
PT:															;;maybe a test function,i don't know.....never used.
	push eax
	push gs
	mov ax,8
	mov gs,ax
	inc byte[gs:0]
	pop gs
	pop eax
	ret
ReloadLDT:														;;reload pointed ldt.
	cli
	push ebp
	mov ebp,esp
	push eax
	mov ax,word[ebp+8]
	lldt ax
	pop eax
	pop ebp
	sti
	ret
LoadCr3:												;;reload cr3 given.
	cli
	push ebp
	mov ebp,esp
	push eax
	push ebx
	mov eax,[ebp+8]
	mov cr3,eax
	pop ebx
	pop eax
	pop ebp
	sti
	ret
