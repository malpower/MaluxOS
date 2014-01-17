Clear: FDT FCDT SAT
	@echo now makefile is going to write files.
	@fwriter ../../IMG/malux005.img FDT 95 1
	@fwriter ../../IMG/malux005.img FCDT 99 1
	@fwriter ../../IMG/malux005.img SAT 189 1
	@echo All clear
FDT: FDT.asm
	@nasm FDT.asm -f bin -o FDT
FCDT: FCDT.asm 
	@nasm FCDT.asm -f bin -o FCDT
SAT: sat.asm
	@nasm sat.asm -f bin -o SAT
