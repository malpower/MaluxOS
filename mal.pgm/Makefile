Debug: mal.pgm
	@fwriter ../../IMG/malux005.img mal.pgm 119 10
mal.pgm: ASM.o C.o main.o
	ld -e MLX_Start -Ttext 0 -Tbss 0x00000B00 --oformat binary ASM.o main.o C.o -o mal.pgm
ASM.o: lib.asm
	nasm -f elf lib.asm -o ASM.o
C.o: lib.c
	gcc -fno-stack-protector -c lib.c -o C.o 
main.o: main.c
	gcc -c main.c -o main.o -fno-stack-protector -Wall
