IMG ?= ../malux005.img

Clear: FDT FCDT SAT
	@echo now makefile is going to write files.
	@dd if=FDT of=$(IMG) bs=512 seek=95 count=1 conv=notrunc status=none
	@dd if=FCDT of=$(IMG) bs=512 seek=99 count=1 conv=notrunc status=none
	@dd if=SAT of=$(IMG) bs=512 seek=189 count=1 conv=notrunc status=none
	@echo All clear
FDT: FDT.asm
	@nasm FDT.asm -f bin -o FDT
FCDT: FCDT.asm 
	@nasm FCDT.asm -f bin -o FCDT
SAT: sat.asm
	@nasm sat.asm -f bin -o SAT
