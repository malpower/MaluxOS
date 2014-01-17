[bits 32]
[section .text]
; Memory manager
global Sys_AllocAPage
global Sys_FreeAPage

; Screen output
global Sys_ClearScreen
global Sys_DrawRect
global Sys_GetInGmode
global Sys_PrintAChar
global Sys_PrintByte
global Sys_SetPixel

; File system
global Sys_CloseFile
global Sys_CreateFile
global Sys_DeleteFile
global Sys_ListFile
global Sys_OpenFile
global Sys_ReadFile
global Sys_WriteFile

; Threads/multitasking
global Sys_CreateThread
global Sys_DestroyThread
global Sys_Execute
global Sys_GetProcessThreads
global Sys_QuitProcess

; Messaging/IPC
global Sys_GetMessage
global Sys_SendMessage

; Core library
global Sys_GetTime
global Sys_PortIn
global Sys_PortOut

; Core HAL
global Sys_ReadKeyBuff
global Sys_Shutdown
global Sys_GetMouse


extern Sys_C_CreateThread
extern KRNL_GetTime
extern Sys_C_ReadKeyBuff
extern Sys_C_PrintByte
extern Sys_C_DestroyThread
extern PrintNumberAsHex
extern Sys_C_Execute
extern Sys_C_PrintAChar
extern Sys_C_LinkPage
extern Sys_C_GetProcessThreads
extern Sys_C_ListFile
extern Sys_C_GetMouse
extern MallocAPage
extern Sys_C_SendMessage
extern Sys_C_GetMessage
extern Sys_C_QuitProcess
extern Sys_C_OpenFile
extern Sys_C_CreateFile
extern Sys_C_DeleteFile
extern Sys_C_UnlinkPage
extern Sys_C_CloseFile
extern Sys_C_ReadFile
extern Sys_C_WriteFile
extern Sys_C_SetPixel
extern Sys_C_ClearScreen
extern Sys_C_DrawRect

Sys_ClearScreen:
	cli
	push ebp
	mov ebp,dword[esp+16]
	push eax
	push ebx
	push ecx
	push edx
	push gs
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov ax,15
	mov es,ax
	mov ax,8
	mov gs,ax
	mov eax,dword[es:ebp]
	mov ebx,dword[es:ebp+4]
	mov ecx,dword[es:ebp+8]
	mov edx,dword[es:ebp+12]
	push edx
	push ecx
	push ebx
	push eax
	mov eax,0x00200000
	mov cr3,eax
	call Sys_C_ClearScreen
	add esp,16
	pop eax
	mov cr3,eax
	pop ds
	pop es
	pop gs
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	sti
	iretd


Sys_DrawRect:
	cli
	push ebp
	mov ebp,dword[esp+16]
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push gs
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov ax,15
	mov es,ax
	mov ax,8
	mov gs,ax
	mov eax,dword[es:ebp]
	mov ebx,dword[es:ebp+4]
	mov ecx,dword[es:ebp+8]
	mov edx,dword[es:ebp+12]
	mov edi,dword[es:ebp+16]
	push edi
	push edx
	push ecx
	push ebx
	push eax
	mov eax,0x00200000
	mov cr3,eax
	call Sys_C_DrawRect
	add esp,20
	pop eax
	mov cr3,eax
	pop ds
	pop es
	pop gs
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	sti
	iretd



Sys_DeleteFile:
	cli
	push ebp
	mov ebp,esp
	push ebx
	push ecx
	push edx
	push edi
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov ax,[ebp+20]
	mov es,ax
	mov ecx,28
	mov ebx,0x0000A000
	mov edi,[ebp+16]
	mov edi,dword[es:edi]
   .bol_Sys_DeleteFile:
	mov al,byte[es:edi]
	mov byte[ds:ebx],al
	inc edi
	inc ebx
	loop .bol_Sys_DeleteFile
	mov al,0
	mov byte[ds:ebx],al
	mov ebx,0x0000A000
	mov eax,0x00200000
	mov cr3,eax
	push ebx
	call Sys_C_DeleteFile
	pop ebx
	pop ebx
	mov cr3,ebx
	pop ds
	pop es
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop ebp
	sti
	iretd



Sys_CreateFile:
	cli
	push ebp
	mov ebp,esp
	push ebx
	push ecx
	push edx
	push edi
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov ax,[ebp+20]
	mov es,ax
	mov ecx,28
	mov ebx,0x0000A000
	mov edi,[ebp+16]
	mov eax,dword[es:edi+4]
	mov dword[fType],eax
	mov edi,dword[es:edi]
   .bol_Sys_CreateFile:
	mov al,byte[es:edi]
	mov byte[ds:ebx],al
	inc edi
	inc ebx
	loop .bol_Sys_CreateFile
	mov al,0
	mov byte[ds:ebx],al
	mov ebx,0x0000A000
	mov eax,0x00200000
	mov cr3,eax
	mov eax,dword[fType]
	push eax
	push ebx
	call Sys_C_CreateFile
	pop ebx
	pop ebx
	pop ebx
	mov cr3,ebx
	pop ds
	pop es
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop ebp
	sti
	iretd
	fType: dd 0

Sys_Shutdown:
	cli
	jmp $
Sys_ListFile:
	cli
	push ebp
	push ebx
	mov ebx,cr3
	push ebx
	push gs
	push ds
	mov ax,24
	mov ds,ax
	mov eax,0x00200000
	mov cr3,eax
	call Sys_C_ListFile
	pop ds
	pop gs
	pop ebx
	mov cr3,ebx
	pop ebx
	pop ebp
	iretd
Sys_GetMessage:
	cli
	push ebp
	mov ebp,dword[esp+16]
	push ebx
	push ecx
	push gs
	push ds
	push es
	mov ax,15
	mov es,ax
	mov ax,24
	mov ds,ax
	mov eax,dword[es:ebp]
	mov dword[_uAddr],eax
	mov ebx,cr3
	push ebx
	mov eax,0x00200000
	mov cr3,eax
	push dword _msg
	call Sys_C_GetMessage
	add esp,4
	pop ebx
	mov cr3,ebx
	mov ebx,dword[_uAddr]
	mov ecx,dword[_msg]
	mov dword[es:ebx],ecx
	mov ecx,dword[_msg+4]
	mov dword[es:ebx+4],ecx
	pop es
	pop ds
	pop gs
	pop ecx
	pop ebx
	pop ebp
	iretd
   _msg:
	dd 0
	dd 0
   _uAddr:
	dd 0
Sys_PrintAChar:						;;a sys call.print a char
	cli
	push ebp
	mov ebp,dword[esp+16]
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push gs
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov ax,15
	mov es,ax
	mov ax,8
	mov gs,ax
	mov eax,dword[es:ebp]
	mov ebx,dword[es:ebp+4]
	mov edx,dword[es:ebp+8]
	mov edi,dword[es:ebp+12]
	push edi
	push edx
	push ebx
	push eax
	mov eax,0x00200000
	mov cr3,eax
	call Sys_C_PrintAChar
	add esp,16
	pop eax
	mov cr3,eax
	pop ds
	pop es
	pop gs
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	sti
	iretd

Sys_SendMessage:						;;a sys call.send a message to another process
	cli
	push ebp
	mov ebp,dword[esp+16]
	push eax
	push ebx
	push ecx
	push gs
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov ax,15
	mov es,ax
	mov ax,8
	mov gs,ax
	mov eax,dword[es:ebp]
	mov ebx,dword[es:ebp+4]
	mov ecx,dword[es:ebp+8]
	push ecx
	push ebx
	push eax
	mov eax,0x00200000
	mov cr3,eax
	call Sys_C_SendMessage
	add esp,12
	pop eax
	mov cr3,eax
	pop ds
	pop es
	pop gs
	pop ecx
	pop ebx
	pop eax
	pop ebp
	sti
	iretd

Sys_SetPixel:
	cli
	push ebp
	mov ebp,dword[esp+16]
	push eax
	push ebx
	push ecx
	push gs
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov ax,15
	mov es,ax
	mov ax,8
	mov gs,ax
	mov eax,dword[es:ebp]
	mov ebx,dword[es:ebp+4]
	mov ecx,dword[es:ebp+8]
	push ecx
	push ebx
	push eax
	mov eax,0x00200000
	mov cr3,eax
	call Sys_C_SetPixel
	add esp,12
	pop eax
	mov cr3,eax
	pop ds
	pop es
	pop gs
	pop ecx
	pop ebx
	pop eax
	pop ebp
	sti
	iretd


Sys_GetInGmode:																	;;a system call,wanted to get into Graphic mode ,but it is failed.because i have no documentation about vga.
	cli
	push eax
	push ebx
	push ds
	mov ax,24
	mov ds,ax
	mov ebx,0x00008100
	mov eax,1
	mov [ebx+80],eax
	pop ds
	pop ebx
	pop eax
	sti
	iretd
Sys_ReadKeyBuff:					;;system call,read a char buff from keyboard buffer,supported by Sys_C_ReadKeyBuff()
	cli
	push ds
	mov ax,24
	mov ds,ax
	call Sys_C_ReadKeyBuff
	pop ds
	iretd

Sys_GetMouse:
	cli
	push ebp
	mov ebp,dword[esp+16]
	push eax
	push ebx
	push ecx
	push gs
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov eax,0x00200000
	mov cr3,eax
	call Sys_C_GetMouse
	pop eax
	mov cr3,eax
	pop ds
	pop es
	pop gs
	pop ecx
	pop ebx
	pop eax
	pop ebp
	sti
	iretd


Sys_PortOut:									;;system call,output a byte to I/O device
	cli
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push edx
	push es
	mov eax,dword[ebp+20]
	mov es,ax
	mov eax,dword[ebp+16]
	mov ebx,eax
	mov eax,dword[es:ebx+4]
	mov edx,dword[es:ebx]
	out dx,al
	pop es
	pop edx
	pop ebx
	pop eax
	pop ebp
	sti
	iretd
Sys_PortIn:											;;system call ,read a byte from I/O device.
	cli
	push ebp
	mov ebp,esp
	push ebx
	push edx
	push es
	mov eax,dword[ebp+20]
	mov es,ax
	mov eax,dword[ebp+16]
	mov ebx,eax
	mov edx,dword[es:ebx]
	xor eax,eax
	in al,dx
	pop es
	pop edx
	pop ebx
	pop ebp
	sti
	iretd
Sys_GetProcessThreads:
	cli
	push ebp
	mov ebp,esp
	push ebx
	push edx
	push es
	push ds
	mov ax,24
	mov ds,ax
	mov eax,dword[ebp+20]
	mov es,ax
	mov eax,dword[ebp+16]
	mov ebx,eax
	mov edx,dword[es:ebx]
	xor eax,eax
	push edx
	call Sys_C_GetProcessThreads
	pop edx
	pop ds
	pop es
	pop edx
	pop ebx
	pop ebp
	sti
	iretd

Sys_FreeAPage:
	cli
	push ebp
	mov ebp,esp
	push eax
	mov eax,cr3
	push eax
	push ebx
	push edx
	push es
	push ds
	mov ax,24
	mov ds,ax
	mov eax,dword[ebp+20]
	mov es,ax
	mov eax,dword[ebp+16]
	mov ebx,eax
	mov edx,dword[es:ebx]
	xor eax,eax
	mov eax,0x00200000
	mov cr3,eax
	push edx
	call Sys_C_UnlinkPage
	pop edx
	pop ds
	pop es
	pop edx
	pop ebx
	pop eax
	mov cr3,eax
	pop eax
	pop ebp
	sti
	iretd

Sys_CloseFile:
	cli
	push ebp
	mov ebp,esp
	push eax
	mov eax,cr3
	push eax
	push ebx
	push edx
	push es
	push ds
	mov ax,24
	mov ds,ax
	mov eax,dword[ebp+20]
	mov es,ax
	mov eax,dword[ebp+16]
	mov ebx,eax
	mov edx,dword[es:ebx]
	mov edx,dword[es:edx+48]
	xor eax,eax
	mov eax,0x00200000
	mov cr3,eax
	push edx
	call Sys_C_CloseFile
	pop edx
	pop ds
	pop es
	pop edx
	pop ebx
	pop eax
	mov cr3,eax
	pop eax
	pop ebp
	sti
	iretd


Sys_ReadFile:
	cli
	push ebp
	mov ebp,esp
	push esi
	mov esi,cr3
	push esi
	push ebx
	push ecx
	push edi
	push edx
	push es
	push ds
	mov bx,24
	mov ds,bx
	mov esi,dword[ebp+20]
	mov es,si
	mov esi,dword[ebp+16]
	mov ebx,esi
	mov edx,dword[es:ebx]
	mov edx,dword[es:edx+48]
	mov ecx,dword[es:ebx+4]
	mov edi,dword[es:ebx+8]
	mov ebx,dword[es:ebx+12]
	mov esi,0x00200000
	mov cr3,esi
	push ebx
	push edi
	push ecx
	push edx
	call Sys_C_ReadFile
	add esp,16
	pop ds
	pop es
	pop edx
	pop edi
	pop ecx
	pop ebx
	pop esi
	mov cr3,esi
	pop esi
	pop ebp
	sti
	iretd


Sys_WriteFile:
	cli
	push ebp
	mov ebp,esp
	push esi
	mov esi,cr3
	push esi
	push ebx
	push ecx
	push edi
	push edx
	push es
	push ds
	mov bx,24
	mov ds,bx
	mov esi,dword[ebp+20]
	mov es,si
	mov esi,dword[ebp+16]
	mov ebx,esi
	mov edx,dword[es:ebx]
	mov edx,dword[es:edx+48]
	mov ecx,dword[es:ebx+4]
	mov edi,dword[es:ebx+8]
	mov ebx,dword[es:ebx+12]
	mov esi,0x00200000
	mov cr3,esi
	push ebx
	push edi
	push ecx
	push edx
	call Sys_C_WriteFile
	add esp,16
	pop ds
	pop es
	pop edx
	pop edi
	pop ecx
	pop ebx
	pop esi
	mov cr3,esi
	pop esi
	pop ebp
	sti
	iretd

Sys_CreateThread:							;;Create a thread,system call,supported by Sys_C_CreateThread().
	cli
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push fs
	push es
	push ds
	mov ax,24
	mov ds,ax
	mov eax,dword[ebp+20]
	mov es,ax
	mov eax,dword[ebp+16]
	mov ebx,eax
	mov edx,dword[es:ebx+4]
	mov ecx,dword[es:ebx]
	push edx
	push ecx
	call Sys_C_CreateThread
	add esp,8
	pop ds
	pop es
	pop fs
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	iretd

Sys_DestroyThread:
	cli
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push ecx
	push ds
	mov ax,24
	mov ds,ax
	call Sys_C_DestroyThread
	pop ds
	pop ecx
	pop ebx
	pop eax
	pop ebp
	iretd
Sys_QuitProcess:
    cli
    push ebp
    mov ebp,esp
    push eax
    push ds
    mov ax,24
    mov ds,ax
    mov eax,cr3
    push eax
    mov eax,0x00200000
    mov cr3,eax
    call Sys_C_QuitProcess
    pop eax
    mov cr3,eax
    pop ds
    pop eax
    pop ebp
    cli
	iretd
Sys_GetTime:
	cli
	iretd
Sys_AllocAPage:
	cli
	push ebx
	mov eax,cr3
	push eax
	push ds
	mov eax,0x00200000
	mov cr3,eax
	mov ax,24
	mov ds,ax
	call MallocAPage
	push eax
	call Sys_C_LinkPage
	add esp,4
	pop ds
	pop ebx
	mov cr3,ebx
	pop ebx
	iretd


Sys_Execute:
	cli
	push ebp
	mov ebp,esp
	push ebx
	push ecx
	push edx
	push edi
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov ax,[ebp+20]
	mov es,ax
	mov ecx,28
	mov ebx,0x0000A000
	mov edi,[ebp+16]
	mov edi,dword[es:edi]
   .bol_Sys_Execute:
	mov al,byte[es:edi]
	mov byte[ds:ebx],al
	inc edi
	inc ebx
	loop .bol_Sys_Execute
	mov al,0
	mov byte[ds:ebx],al
	mov ebx,0x0000A000
	mov eax,0x00200000
	mov cr3,eax
	push ebx
	call Sys_C_Execute
	pop ebx
	pop ebx
	mov cr3,ebx
	pop ds
	pop es
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop ebp
	sti
	iretd

Sys_OpenFile:
	cli
	push ebp
	mov ebp,esp
	push ebx
	push ecx
	push edx
	push edi
	push es
	push ds
	mov eax,cr3
	push eax
	mov ax,24
	mov ds,ax
	mov ax,[ebp+20]
	mov es,ax
	mov ecx,28
	mov ebx,0x0000A000
	mov edi,[ebp+16]
	mov edi,dword[es:edi]
   .bol_Sys_OpenFile:
	mov al,byte[es:edi]
	mov byte[ds:ebx],al
	inc edi
	inc ebx
	loop .bol_Sys_OpenFile
	mov al,0
	mov byte[ds:ebx],al
	mov ebx,0x0000A000
	mov eax,0x00200000
	mov cr3,eax
	push ebx
	call Sys_C_OpenFile
	pop ebx
	pop ebx
	mov cr3,ebx
	pop ds
	pop es
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop ebp
	sti
	iretd


