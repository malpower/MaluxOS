[bits 32]
[section .text]
global MLX_PrintChar
global MLX_CreateThread_R
global MLX_ReadKeyBuff
global MLX_PortOut
global MLX_PortIn
global MLX_Start
global MLX_Execute
global MLX_MAlloc
global MLX_SetPixel
global MLX_DiscardCPU
global MLX_WaitProcess
global MLX_ListFile
global MLX_Shutdown
global MLX_SendMessage
global MLX_GetMessageASM
global MLX_CreateFile
global MLX_DeleteFile
global MLX_OpenFile
global MLX_CloseFile
global MLX_ReadFile
global MLX_WriteFile
global MLX_MFree
global MLX_ClearScreen
global MLX_DrawRect
extern MlxMain
MLX_Start:
	call MlxMain
	call MLX_Exit
MLX_CreateFile:
	push ebp
	mov ebp,esp
	mov eax,dword[ebp+12]
	push eax
	mov eax,dword[ebp+8]
	push eax
	int 0xD0
	add esp,8
	pop ebp
	ret
MLX_OpenFile:
	push ebp
	mov ebp,esp
	mov eax,dword[ebp+8]
	push eax
	int 0xD2
	add esp,4
	pop ebp
	ret
MLX_CloseFile:
	push ebp
	mov ebp,esp
	push eax
	mov eax,dword[ebp+8]
	push eax
	int 0xD3
	add esp,4
	pop eax
	pop ebp
	ret
MLX_ReadFile:
	push ebp
	mov ebp,esp
	push ebx
	mov ebx,dword[ebp+20]
	push ebx
	mov ebx,dword[ebp+16]
	push ebx
	mov ebx,dword[ebp+12]
	push ebx
	mov ebx,dword[ebp+8]
	push ebx
	int 0xD4
	add esp,16
	pop ebx
	pop ebp
	ret
MLX_WriteFile:
	push ebp
	mov ebp,esp
	push ebx
	mov ebx,dword[ebp+20]
	push ebx
	mov ebx,dword[ebp+16]
	push ebx
	mov ebx,dword[ebp+12]
	push ebx
	mov ebx,dword[ebp+8]
	push ebx
	int 0xD5
	add esp,16
	pop ebx
	pop ebp
	ret
MLX_MFree:
	push ebp
	mov ebp,esp
	push eax
	mov eax,dword[ebp+8]
	push eax
	int 0xBA
	add esp,4
	pop eax
	pop ebp
	ret
MLX_DeleteFile:
	push ebp
	mov ebp,esp
	mov eax,dword[ebp+8]
	push eax
	int 0xD1
	add esp,4
	pop ebp
	ret
MLX_PrintChar:
	push ebp
	mov ebp,esp
	push eax
	push ecx
	push edx
	mov eax,dword[ebp+20]
	push eax
	mov eax,dword[ebp+16]
	push eax
	mov eax,dword[ebp+12]
	push eax
	mov eax,dword[ebp+8]
	push eax
	int 0xA4
	add esp,16
	pop edx
	pop ecx
	pop eax
	pop ebp
	ret
MLX_LIB_ExitThread:
	int 0xB6
	jmp $
MLX_CreateThread_R:
	push ebp
	mov ebp,esp
	push eax
	push ebx
	mov eax,dword[ebp+12]
	push eax
	mov ebx,MLX_LIB_ExitThread
	mov dword[ds:eax],ebx
	mov eax,dword[ebp+8]
	push eax
	int 0xB5
	add esp,8
	pop ebx
	pop eax
	pop ebp
	ret
MLX_ReadKeyBuff:
	int 0xA2
	ret
MLX_PortIn:
	push ebp
	mov ebp,esp
	mov eax,dword[ebp+8]
	push eax
	int 0xA1
	add esp,4
	pop ebp
	ret
MLX_PortOut:
	push ebp
	mov ebp,esp
	push eax
	mov eax,dword[ebp+12]
	push eax
	mov eax,dword[ebp+8]
	push eax
	int 0xA0
	add esp,8
	pop eax
	pop ebp
	ret
MLX_Exit:
	int 0xB2
	jmp $
MLX_Execute:
	push ebp
	mov ebp,esp
	mov eax,dword[ebp+8]
	push eax
	int 0xB4
	add esp,4
	pop ebp
	ret
MLX_MAlloc:
	int 0xB3
	ret
MLX_DiscardCPU:
	int 0x20
	ret
MLX_WaitProcess:
	push ebp
	mov ebp,esp
	push dword[ebp+8]
	int 0xB7
	add esp,4
	pop ebp
	ret
MLX_SendMessage:
	push ebp
	mov ebp,esp
	push dword[ebp+16]
	push dword[ebp+12]
	push dword[ebp+8]
	int 0xB8
	add esp,12
	pop ebp
	ret
MLX_ClearScreen:
	push ebp
	mov ebp,esp
	push dword[ebp+20]
	push dword[ebp+16]
	push dword[ebp+12]
	push dword[ebp+8]
	int 0xA7
	add esp,16
	pop ebp
	ret
MLX_DrawRect:
	push ebp
	mov ebp,esp
	push dword[ebp+24]
	push dword[ebp+20]
	push dword[ebp+16]
	push dword[ebp+12]
	push dword[ebp+8]
	int 0xA8
	add esp,20
	pop ebp
	ret
MLX_SetPixel:
	push ebp
	mov ebp,esp
	push dword[ebp+16]
	push dword[ebp+12]
	push dword[ebp+8]
	int 0xA6
	add esp,12
	pop ebp
	ret
MLX_GetMessageASM:
	push ebp
	mov ebp,esp
	push dword[ebp+8]
	int 0xB9
	add esp,4
	pop ebp
	ret
MLX_ListFile:
	int 0xA5
	ret
MLX_Shutdown:
	int 0xC2
	jmp $

