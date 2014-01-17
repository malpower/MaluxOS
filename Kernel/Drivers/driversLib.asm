[bits 32]
[section .text]
extern Driver_C_KeyBoard
global Driver_KeyBoard
global Driver_HD_Interruption
global Driver_HD_ReadSector
global Driver_HD_WriteSector
global Driver_Mouse
extern PrintNumberAsHex
extern X_TEST
global Driver_HD_InitHDEnv
extern Driver_HD_C_Interruption
extern Driver_HD_C_Interruption
global dRwAddress
global dRwSwitch
dRwAddress dd 0
dRwSwitch db 0
Driver_KeyBoard:														;;KB driver as asm ,it will call KB driver made in C.
	cli
	push eax
	push ebx
	push ecx
	push edx
	push ds
	mov ax,24
	mov ds,ax
	call Driver_C_KeyBoard
	pop ds
	pop edx
	pop ecx
	pop ebx
	mov al,0x20
	out 0x20,al
	pop eax
	sti
	iretd

Driver_Mouse:
	cli
	pushad
	push ds
	mov ax, 24
	mov ds, ax



	mov al, 0x20
	out 0x20, al
	out 0xA0, al

	pop ds
	popad

	sti
	iretd

Driver_HD_Interruption:									;;HD driver as asm ,it will call HD dirver made in C.
	cli
	push eax
	push edx
	push ecx
	push ebx
	push edi
	push es
	push ds
	mov ax,24
	mov ds,ax
	mov dx,0x1F7
	xor eax,eax
	mov al,byte[dRwSwitch]
	push eax
	xor eax,eax
	in al,dx
	push eax
	call Driver_HD_C_Interruption
	add esp,8
	pop ds
	pop es
	pop edi
	pop ebx
	pop ecx
	pop edx
	mov al,0x20
	out 0x20,al
	out 0xA0,al
	pop eax
	sti
	iretd
Driver_HD_ReadSector:															;;Sector read function.									
	cli
	push eax
	push edx
	push ecx
	push ebx
	push edi
	push es
	push ds
	mov ax,24
	mov ds,ax
	mov ax,64
	mov es,ax
	mov ebx,dword[dRwAddress]
	mov ecx,128
   .bol_Driver_HD_ReadSector_1:
	mov dx,0x1F7
	in al,dx
	cmp al,0x58
	jne .bol_Driver_HD_ReadSector_1
	mov dx,0x1F0
	in eax,dx
	mov dword[es:ebx],eax
	add ebx,4
	loop .bol_Driver_HD_ReadSector_1
	pop ds
	pop es
	pop edi
	pop ebx
	pop ecx
	pop edx
	pop eax
	sti
	ret
Driver_HD_WriteSector:									;;hd sector write function.
	cli
	push eax
	push edx
	push ecx
	push ebx
	push edi
	push es
	push ds
	mov ax,24
	mov ds,ax
	mov ax,64
	mov es,ax
	mov ebx,dword[dRwAddress]
	mov ecx,128
   .bol_Driver_HD_ReadSector_1:
	mov dx,0x1F7
	in al,dx
	cmp al,0x58
	jne .bol_Driver_HD_ReadSector_1
	mov dx,0x1F0
	mov eax,dword[es:ebx]
	out dx,eax
	add ebx,4
	loop .bol_Driver_HD_ReadSector_1
	pop ds
	pop es
	pop edi
	pop ebx
	pop ecx
	pop edx
	pop eax
	sti
	ret
Driver_HD_InitHDEnv:									;;no use here.maybe useful in future.
	
	ret
