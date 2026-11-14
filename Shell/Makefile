IMG ?= ../malux005.img

Debug: EUI
	@dd if=EUI of=$(IMG) bs=512 seek=100 count=19 conv=notrunc status=none
EUI: ASM.o C.o main.o
	@ld -e MLX_Start -Ttext 0 -Tbss 0x00001E00 --oformat binary ASM.o main.o C.o -o EUI
ASM.o: lib.asm
	@nasm -f elf lib.asm -o ASM.o
C.o: lib.c
	@gcc -fno-stack-protector -c lib.c -o C.o 
main.o: main.c
	@gcc -c main.c -o main.o -fno-stack-protector -Wall
