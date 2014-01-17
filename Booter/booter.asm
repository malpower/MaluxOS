org 0x7C00
jmp main
@GDT:
	GDT_NULL:
		dw 0
		dw 0
		dw 0
		dw 0
	GDT_VIDEO:
		dw 0xFFFF
		dw 0x8000
		db 0x0B
		dw 0x40F2
		db 0
	GDT_CODE32:
		dw 0xFFFF
		dw 0x0000
		db 0x10
		dw 0x4098
		db 0
	GDT_CODE32_DATA:
		dw 0xFFFF
		dw 0x0000
		db 0x10
		dw 0x4092
		db 0
	GDT_PDE_TABLE:
		dw 4095
		dw 0x0000
		db 0x20
		dw 0x4092
		db 0
	GDT_PTE_TABLE:
		dw 1024
		dw 0x1000
		db 0x20
		dw 0xC092
		db 0
	GDT_FOR_qqq:
		dw 0xFFFF
		dw 0
		db 0
		dw 0x4092
		db 0
	GDT_FOR_KERNEL:
		dw 0xFFFF
		dw 0
		db 0
		dw 0x4098
		db 0
	GDT_TMP:
		dw 0xFFFF
		dw 0x0000
		db 0x01
		dw 0x4092
		db 0
	gdtLen	equ  $-GDT_NULL
	gdtPtr:
		dw gdtLen-1
		dd GDT_NULL
strWelcome:
	db "maLux is now booting......."
	db 0
strLoading:
	db "Kernel is now loading......."
	db 0
strJmp:
	db "Jumping into Protected Mode..."
	db 0
[bits 16]
PrintString:
	push ebp
	mov ebp,esp
	push eax
	push ebx
	push ecx
	push edx
	push gs
	xor edx,edx
	xor ecx,ecx
	mov cx,word[ebp+6]
	mov dx,word[ebp+8]
	mov ebx,0
	mov ax,dx
	mov dx,160
	mul dx
	mov dx,ax
	add ebx,edx
	mov ax,0xB800
	mov gs,ax
	mov ah,0x0F
   .bol_PrintString:
	cmp byte[ds:ecx],0
	je .eol_PrintString
	mov al,byte[ds:ecx]
	mov [gs:ebx],ax
	add ebx,2
	inc cx
	jmp .bol_PrintString
   .eol_PrintString:
	pop gs
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret
main:
	mov ax,cs
	mov ds,ax
	mov ss,ax
	mov es,ax
	mov eax,0x7C00
	mov esp,eax
	mov ax,strWelcome
	push word 0
	push word ax
	call PrintString
	add esp,4
	mov ah,0
	mov dl,0
	int 0x13
	push word 1
	mov ax,strLoading
	push word ax
	call PrintString
	add esp,4
	xor eax,eax
	mov ax,cs
	shl eax,4
	add eax,C32
	mov word[GDT_FOR_KERNEL+2],ax
	shr eax,16
	mov byte[GDT_FOR_KERNEL+4],al
	mov byte[GDT_FOR_KERNEL+7],ah
	mov ax,0x1000
	mov es,ax
	mov ebx,0	
	mov ah,2
	mov al,64
	mov cl,2
	mov dl,0
	mov dh,0
	mov ch,0
	int 0x13
	mov ax,strJmp
	push word 2
	push word ax
	call PrintString
	add esp,4
	mov ah,0x88
	int 0x15
	mov word[es:0x7FF0],ax
	cli
	in al,0x92
	or al,2
	out 0x92,al
	mov eax,cr0
	or eax,1
	mov cr0,eax
	lgdt [gdtPtr]
	jmp dword 56:0
[bits 32]
C32:
	mov ecx,0xFFFF
	mov ax,64
	mov es,ax
	mov ax,24
	mov fs,ax
	mov ebx,0
   .bol_32:
	mov al,byte[es:ebx]
	mov byte[fs:ebx],al
	add ebx,1
	loop .bol_32
	mov eax,0xFFF0
	mov esp,eax
	mov ax,24
	mov ds,ax
	mov es,ax
	mov ss,ax
	jmp dword 16:0x0
times (510-($-$$)) db 0
db 0x55
db 0xAA
