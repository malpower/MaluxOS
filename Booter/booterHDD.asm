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
		db 0x0
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
	GDT_FOR_NGDT:
		dw 0xFFFF
		dw 0
		db 0x03
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
[bits 16]
main:
	mov ax,cs
	mov ds,ax
	mov ax,0x9000
	mov es,ax
	mov di,0
	mov ax,0x4F01
	mov cx,0x118
	int 0x10
	mov ax,0x4F02
	mov bx,0x4118
	int 0x10
	mov ax,0
	mov es,ax
	mov bx,0x104
	mov bx,word[es:bx+2]
	mov ax,word[es:bx]
	mov es,ax
	mov ax,word[es:bx+5]
	mov bx,0x1000
	mov es,bx
	mov word[es:0x7FFA],ax
	mov ax,cs
	mov ds,ax
	mov ss,ax
	mov es,ax
	mov eax,0x7C00
	mov esp,eax
	mov ah,0
	mov dl,0
	int 0x13
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
	mov dl,0x80
	mov dh,0
	mov ch,0
	int 0x13
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
