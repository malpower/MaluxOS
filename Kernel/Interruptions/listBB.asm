[bits 32]
[section .text]
global INT_CLOCK
global INT_TEST
global INT_GP
extern ProcessDispatcher
INT_CLOCK:
	cli
	push ebx
	push eax
	push ds
	mov ax,24
	mov ds,ax
	pop ax
	mov ebx,0x00008000
	mov word[ebx+40],ax
	pop eax
	mov dword[ebx+4],eax
	mov dword[ebx],ebp
	pop eax
	mov dword[ebx+8],eax
	mov dword[ebx+12],ecx
	mov dword[ebx+16],edx
	mov dword[ebx+20],edi
	mov dword[ebx+24],esi
	mov ebp,esp
	mov eax,dword[ebp+2]
	mov dword[ebx+32],eax
	mov ax,word[ebp+6]
	mov word[ebx+38],ax
	mov eax,dword[ebp+10]
	mov dword[ebx+34],eax
	mov eax,dword[ebp+14]
	mov dword[ebx+28],eax
	mov ax,word[ebp+18]
	mov word[ebx+48],ax
	mov ax,es
	mov word[ebx+42],ax
	mov ax,gs
	mov word[ebx+44],ax
	mov ax,fs
	mov word[ebx+46],ax
	call ProcessDispatcher
	mov ebx,0x00008000
	mov ecx,dword[ebx+12]
	mov edx,dword[ebx+16]
	mov edi,dword[ebx+20]
	mov esi,dword[ebx+24]
	mov ax,word[ebx+42]
	mov es,ax
	mov ax,word[ebx+44]
	mov gs,ax
	mov ax,word[ebx+46]
	mov fs,ax
	mov ebp,esp
	mov eax,dword[ebx+28]
	mov dword[ebp+14],eax
	mov eax,dword[ebx+32]
	mov dword[ebp+2],eax
	mov eax,dword[ebx+34]
	mov dword[ebp+10],eax
	mov ax,word[ebx+38]
	mov word[ebp+6],ax
	mov ebp,dword[ebx+0]
	mov eax,dword[ebx+4]
	xor eax,eax
	mov ax,word[ebx+40]
	push eax
	mov ebx,dword[ebx+8]
	pop ds
	push ax
	mov al,0x20
	out 0x20,al
	pop ax
	sti
	iretd
INT_TEST:
	cli
	mov ax,8
	mov gs,ax
	mov ah,0x0C
	mov al,'X'
	mov [gs:0],ax
	sti
	iretd
INT_GP:	
	cli
	mov ax,8
	mov gs,ax
	mov ah,0x1C
	mov al,'#'
	mov ebx,0
	mov [gs:ebx],ax
	mov al,'G'
	mov ebx,2
	mov [gs:ebx],ax
	mov al,'P'
	mov ebx,4
	mov [gs:ebx],ax
	jmp $
