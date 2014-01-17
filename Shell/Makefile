Debug: EUI
	@fwriter ../../IMG/malux005.img EUI 100 19
EUI: ASM.o C.o main.o
	@ld -e MLX_Start -Ttext 0 -Tbss 0x00001E00 --oformat binary ASM.o main.o C.o -o EUI
ASM.o: lib.asm
	@nasm -f elf lib.asm -o ASM.o
C.o: lib.c
	@gcc -fno-stack-protector -c lib.c -o C.o 
main.o: main.c
	@gcc -c main.c -o main.o -fno-stack-protector -Wall
