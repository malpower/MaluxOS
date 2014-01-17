[bits 32]
[section .text]
global CreatePageInfo
global CopyToNewGDTArea
global ReloadGDT
global CreateIDT 
global LoadTSS
global IntTest
global CreateTSS
global LoadIDT
global InitPCBs
global InstallFunctionToInterruption
global ActivateMouse
extern GetMemSize
extern PrintNumberAsHex
extern PrintString
gdtPtr:
	dw 0
	dd 0
idtPtr:
	dw 0
	dd 0
CopyToNewGDTArea:									;;Write a byte into a places where store new gdt.
	push ebp
	mov ebp,esp
	push ebx
	push ecx
	push edx
	push es
	mov ax,48
	mov es,ax
	mov eax,[ebp+8]
	mov edx,eax
	mov eax,[ebp+12]
	mov ecx,eax
	mov [es:ecx],dl
	pop es
	pop edx
	pop ecx
	pop ebx
	pop ebp
	ret
CreatePageInfo:								;;create page informations.
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push gs
	push es
	mov eax,dword[ebp+8]
	mov ecx,4
	mov edx,0
	div ecx
	mov edx,eax
	xor eax,eax
	mov ax,32
	mov es,ax
	mov ebx,0
	mov ecx,1023
	mov eax,0x00201007
   .bol_CreatePageInfo_1:
	mov [es:ebx],eax
	add ebx,4
	add eax,4096
	loop .bol_CreatePageInfo_1
	mov eax,0x00201407
	mov [es:ebx],eax
	mov ax,40
	mov es,ax
	mov ebx,0
	mov ecx,edx
	mov eax,0
	mov edi,7
   .bol_CreatePageInfo_2:
	add eax,edi
	mov [es:ebx],eax
	sub eax,edi
	add ebx,4
	add eax,4096
	loop .bol_CreatePageInfo_2
	mov ecx,1024*1024
	sub ecx,edx
	mov edi,6
   .bol_CreatePageInfo_3:
	add eax,edi
	mov [es:ebx],eax
	sub eax,edi
	add ebx,4
	add eax,4096
	loop .bol_CreatePageInfo_3
	mov eax,0x00200000
	mov cr3,eax
	mov eax,cr0
	mov ebx,1
	shl ebx,31
	or eax,ebx
	mov cr0,eax
	pop es
	pop gs
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret
ReloadGDT:											;;reload gdt ,means load new gdt into gdtr
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push gs
	mov eax,[ebp+8]
	mov ebx,8
	mul ebx
	cli
	mov word[gdtPtr],ax
	mov eax,0x00030000
	mov dword[gdtPtr+2],eax
	lgdt [gdtPtr]
	mov ax,24
	mov ss,ax
	mov ds,ax
	pop gs
	pop ebx
	pop eax
	pop ebp
	ret
CreateIDT:											;;create a idt
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push ecx
	push edi
	push es
	xor eax,eax
	mov ax,32
	mov es,ax
	mov eax,16
	shl eax,16
	mov ecx,FoolInterruption
	mov ax,cx
	mov ecx,FoolInterruption
	mov cx,0xEE00
	mov edi,ecx
	mov ecx,256
	mov ebx,0
   .bolA_CreateIDT:
	mov [es:ebx],eax
	add ebx,4
	mov [es:ebx],edi
	add ebx,4
	loop .bolA_CreateIDT 
	pop es
	pop edi
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret
FoolInterruption:										;;fool interruption,it just simply return,we need this to process those unused interruption numbers.
	cli
	push eax
	mov al,0x20
	out 0x20,al
	sti
	pop eax
	iret
CreateTSS:											;;create a tss for a cpu
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push es
	mov ax,48
	mov es,ax
	mov ecx,106
	mov ebx,0
	mov al,0
   .bol_CreateTSS_1:
	mov [es:ebx],al
	inc ebx
	loop .bol_CreateTSS_1
   .eol_CreateTSS_1:
	mov eax,0x0000FFF0
	mov ebx,4
	mov dword[es:ebx],eax
	mov eax,24
	mov ebx,8
	mov [es:ebx],eax
	mov ax,104
	mov ebx,102
	mov [es:ebx],ax
	mov ax,0xFF
	mov ebx,104
	mov [es:ebx],ax
	pop es
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret
LoadTSS:								;;load tss address into tr
	push eax
	mov ax,40
	ltr ax
	pop eax
	ret	
LoadIDT:											;;load idt address into idtr
	push eax
	push ebx
	push ecx
	mov al,0x11
	out 0x20,al
	out 0xA0,al
	mov al,0x20
	out 0x21,al
	mov al,0x28
	out 0xA1,al
	mov al,4
	out 0x21,al
	mov al,2
	out 0xA1,al
	mov al,1
	out 0x21,al
	out 0xA1,al
	mov al,0xF8
	out 0x21,al
	mov al,0xBF
	out  0xA1,al
	mov ax,8*256-1
	mov word[idtPtr],ax
	mov eax,0x00140000
	mov dword[idtPtr+2],eax
	lidt [idtPtr]
	sti
	pop ecx
	pop ebx
	pop eax
	ret
InstallFunctionToInterruption:							;;a function for installing functions onto a interruption place.
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push ecx
	push es
	mov ebx,[ebp+8]
	mov eax,[ebp+12]
	mov ecx,eax
	mov eax,16
	shl eax,16
	mov ax,cx
	mov ecx,eax
	mov eax,ebx
	mov ebx,8
	mul ebx
	mov ebx,eax
	mov ax,32
	mov es,ax
	mov [es:ebx],ecx
	add ebx,4
	mov ecx,0xEE00
	mov [es:ebx],ecx
	pop es
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret

InitPCBs:											;;initialize PCB link,at first time,we used this to create a PCB which use for UI.
	push eax
	push ebx
	push ecx
	push es
	mov ax,24
	mov es,ax
	mov ebx,0x00008000
	mov eax,0
	mov ecx,32
   .bol_InitPCBs_1:
	mov dword[es:ebx+0x44],eax
	mov dword[es:ebx+80],eax
	add ebx,0x100
	loop .bol_InitPCBs_1
	mov ebx,0x00008144
	mov eax,2
	mov dword[es:ebx],eax
	sub ebx,4
	mov eax,80
	mov dword[es:ebx],eax
	add ebx,12
	mov eax,0
	mov dword[es:ebx],eax
	mov ebx,0x00008100
	mov eax,7
	mov dword[es:ebx+4],eax
	mov eax,15
	mov dword[es:ebx+16],eax
	mov eax,15
	mov dword[es:ebx+40],eax
	mov eax,0x206
	mov dword[es:ebx+8],eax
	mov eax,0x00200000
	mov dword[es:ebx+84],eax
	pop es
	pop ecx
	pop ebx
	pop eax
	ret

WaitforKBDRead:
	in al, 0x64
	and al, 0x01
	jz WaitforKBDRead
	ret

WaitforKBDWrite:
	in al, 0x64
	and al, 0x02
	jnz WaitforKBDWrite
	ret

ActivateMouse:
	push ebp
	mov ebp, esp

	cli

	.start_ActivateMouse:
		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xFF
		out 0x60, al

		call WaitforKBDRead

		in al, 0x60
		cmp al, 0xFA
		jne .next_ActivateMouse

		call WaitforKBDRead

		in al, 0x60

	.next_ActivateMouse:
		cmp al, 0xAA
		;jne .start_ActivateMouse

		call WaitforKBDRead

		in al, 0x60
		mov [ebp-4], al

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF3
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xC8
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF3
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0x64
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF3
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0x50
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF2
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDRead
		in al, 0x60

		cmp al, [ebp-4]
		jne .scrmouse_ActivateMouse
		
		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF4
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		mov eax, 0

		leave
		ret

	.scrmouse_ActivateMouse:
		mov [ebp-4], al
		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF3
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xC8
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF3
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xC8
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF3
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0x50
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF2
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		call WaitforKBDRead
		in al, 0x60

		cmp al, [ebp-4]
		jne .5btn_ActivateMouse

		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF4
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		mov eax, 1

		leave
		ret

	.5btn_ActivateMouse:
		call WaitforKBDWrite
		mov al, 0xD4
		out 0x64, al

		call WaitforKBDWrite
		mov al, 0xF4
		out 0x60, al

		call WaitforKBDRead
		in al, 0x60

		mov eax, 2

		leave
		ret
