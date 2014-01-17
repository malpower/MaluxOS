[bits 32]
[section .text]
global INT_CLOCK
global INT_SEUI
global INT_GP
global INT_PF
global INT_SS
extern StartEUI
extern ProcessDispatcher
extern PrintNumberAsHex
extern Error_GP
INT_PF:							;;int for #PF(int 0x0E,memory page fault).
	cli

	jmp $
INT_SEUI:							;; a test int,use to create a process here.
	cli
	push eax
	mov eax,cr3
	push eax
	push ds
	mov ax,24
	mov ds,ax
	call StartEUI
	pop ds
	pop eax
	mov cr3,eax
	pop eax
	iretd
INT_GP:								;;int #GP(int 0x0D,General Protect Fault).
	cli
    push eax
    mov ax,24
    mov ds,ax
    call Error_GP
	mov al,0x20
    out 0x20,al
    pop eax
    sti
    iretd
INT_SS:								;;int #SS(int 0x0C,Stack Segment Fault).
	cli
	jmp $

INT_CLOCK:								;;int 0x20,signal comes form intel 8253/8254.,we call ProcessDispatcher here.

	cli
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push es
	mov eax,24
	mov es,eax
	mov ebx,0x00008000
	mov eax,dword[ebp+4]
	mov dword[es:ebx+0],eax
	mov eax,dword[ebp+8]
	mov dword[es:ebx+4],eax
	mov eax,dword[ebp+12]
	mov dword[es:ebx+8],eax
	mov eax,dword[ebp+16]
	mov dword[es:ebx+12],eax
	mov eax,dword[ebp+20]
	mov dword[es:ebx+16],eax
	mov eax,dword[ebp+0]
	mov dword[es:ebx+20],eax
	mov eax,ecx
	mov dword[es:ebx+24],eax
	mov eax,edx
	mov dword[es:ebx+28],eax
	mov eax,edi
	mov dword[es:ebx+32],eax
	mov eax,esi
	mov dword[es:ebx+36],eax
	mov eax,ds
	mov dword[es:ebx+40],eax
	mov eax,dword[ebp-12]
	mov dword[es:ebx+44],eax
	mov eax,dword[ebp-4]
	mov dword[es:ebx+48],eax
	mov eax,dword[ebp-8]
	mov dword[es:ebx+52],eax
	mov eax,gs
	mov dword[es:ebx+56],eax
	mov eax,fs
	mov dword[es:ebx+60],eax
	mov ax,24
	mov ds,ax
	pop es
	pop ebx
	pop eax
	call ProcessDispatcher
	push eax
	push ebx
	push es
	mov ax,24
	mov es,ax
	mov ebx,0x00008000
	mov eax,dword[es:ebx+0]
	mov dword[ebp+4],eax
	mov eax,dword[es:ebx+4]
	mov dword[ebp+8],eax
	mov eax,dword[es:ebx+8]
	mov dword[ebp+12],eax
	mov eax,dword[es:ebx+12]
	mov dword[ebp+16],eax
	mov eax,dword[es:ebx+16]
	mov dword[ebp+20],eax
	mov eax,dword[es:ebx+20]
	mov dword[ebp+0],eax
	mov eax,dword[es:ebx+24]
	mov ecx,eax
	mov eax,dword[es:ebx+28]
	mov edx,eax
	mov eax,dword[es:ebx+32]
	mov edi,eax
	mov eax,dword[es:ebx+36]
	mov esi,eax
	mov eax,dword[es:ebx+40]
	mov ds,eax
	mov eax,dword[es:ebx+44]
	mov dword[ebp-12],eax
	mov eax,dword[es:ebx+48]
	mov dword[ebp-4],eax
	mov eax,dword[es:ebx+52]
	mov dword[ebp-8],eax
	mov eax,dword[es:ebx+56]
	mov gs,eax
	mov eax,dword[es:ebx+60]
	mov fs,eax
	pop es
	pop ebx
	pop eax
	pop ebp
	push ax
	mov al,0x20
	out 0x20,al
	pop ax
	sti
	iretd






