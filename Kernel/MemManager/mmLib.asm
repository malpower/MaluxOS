[bits 32]
[section .text]
global WriteMMap
global ReadMMap
WriteMMap:
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push es
	mov ax,72
	mov es,ax
	mov eax,dword[ebp+12]   
	mov ebx,dword[ebp+8]
	mov byte[es:ebx],al
	pop es
	pop ebx
	pop eax
	pop ebp
	ret
ReadMMap:
	push ebp
	mov ebp,esp
	push ebx
	push es
	mov ax,72
	mov es,ax
	xor eax,eax
	mov ebx,dword[ebp+8]
	mov al,byte[es:ebx]
	pop es
	pop ebx
	pop ebp
	ret

