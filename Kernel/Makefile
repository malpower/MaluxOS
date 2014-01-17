Debug: kernel
	@echo Writing hard disk image:
	@fwriter ../../IMG/malux005.img ./kernel 1 64
	@echo Buiding finished............
kernel:main.o support.o lib.o Init/Init.o Init/InitLib.o Interruptions/ints.o ProcessDispatcher/ProcessDispatcher.o ProcessDispatcher/ProcessDispatcherLib.o Interruptions/list.o sysCalls/sysCallsLib.o sysCalls/sysCalls.o Drivers/drivers.o Drivers/driversSupports.o Drivers/driversLib.o MemManager/mmLib.o FSystem/malFS.o MemManager/mmLib_C.o
	@echo linking obj files
	@ld -Ttext 0 -Tbss 0xF000 --oformat binary -e main main.o support.o lib.o Init/Init.o Interruptions/ints.o Init/InitLib.o ProcessDispatcher/ProcessDispatcher.o ProcessDispatcher/ProcessDispatcherLib.o Drivers/drivers.o Drivers/driversLib.o Drivers/driversSupports.o sysCalls/sysCallsLib.o sysCalls/sysCalls.o Interruptions/list.o MemManager/mmLib.o FSystem/malFS.o MemManager/mmLib_C.o -o kernel
main.o:main.c
	@echo compiling main.c
	@gcc -fno-stack-protector -Wall -c main.c -fno-stack-protector -o main.o
support.o:support.c
	@echo compiling support.c
	@gcc -fno-stack-protector -Wall -c support.c -fno-stack-protector -o support.o
lib.o:lib.asm
	@echo compiling lib.asm
	@nasm -f elf lib.asm -o lib.o
Init/Init.o:Init/Init.c
	@echo compiling Init.c
	@gcc -fno-stack-protector -Wall -O0 -c Init/Init.c -o Init/Init.o
Init/InitLib.o:Init/InitLib.asm
	@echo compiling InitLib.asm
	@nasm -f elf Init/InitLib.asm -o Init/InitLib.o
ProcessDispatcher/ProcessDispatcher.o: ProcessDispatcher/ProcessDispatcher.c
	@echo compiling ProcessDispatcher.c
	@gcc -fno-stack-protector -Wall -c ProcessDispatcher/ProcessDispatcher.c -o ProcessDispatcher/ProcessDispatcher.o
ProcessDispatcher/ProcessDispatcherLib.o: ProcessDispatcher/ProcessDispatcherLib.asm
	@echo compiling ProcessDispatcherLib.asm
	@nasm -f elf ProcessDispatcher/ProcessDispatcherLib.asm -o ProcessDispatcher/ProcessDispatcherLib.o
Interruptions/list.o: Interruptions/list.asm
	@echo compiling list.asm
	@nasm -f elf Interruptions/list.asm -o Interruptions/list.o
sysCalls/sysCallsLib.o: sysCalls/sysCallsLib.asm
	@echo compiling sysCallsLib.asm
	@nasm -f elf sysCalls/sysCallsLib.asm -o sysCalls/sysCallsLib.o
sysCalls/sysCalls.o: sysCalls/sysCalls.c
	@echo compiling sysCalls.c
	@gcc -fno-stack-protector -Wall -c sysCalls/sysCalls.c -o sysCalls/sysCalls.o
Drivers/drivers.o: Drivers/drivers.c
	@echo compiling drivers.c
	@gcc -fno-stack-protector -Wall -c Drivers/drivers.c -o Drivers/drivers.o -O2
Drivers/driversLib.o: Drivers/driversLib.asm
	@echo compiling driversLib.asm
	@nasm -f elf Drivers/driversLib.asm -o Drivers/driversLib.o
Drivers/driversSupports.o: Drivers/driversSupports.c
	@echo compiling driversSupport.c
	@gcc -fno-stack-protector -Wall -c -O2 Drivers/driversSupports.c -o Drivers/driversSupports.o
MemManager/mmLib.o: MemManager/mmLib.asm 
	@echo compiling mmLib.asm
	@nasm -f elf MemManager/mmLib.asm -o MemManager/mmLib.o
MemManager/mmLib_C.o: MemManager/mmLib_C.c
	@echo compiling mmLib_C.c
	@gcc -fno-stack-protector -Wall -c MemManager/mmLib_C.c -o MemManager/mmLib_C.o
FSystem/malFS.o: FSystem/malFS.c
	@echo compiling malFS.c
	@gcc -fno-stack-protector -Wall -c FSystem/malFS.c -o FSystem/malFS.o
Interruptions/ints.o: Interruptions/ints.c
	@echo compiling ints.c
	@gcc -fno-stack-protector -Wall -c Interruptions/ints.c -o Interruptions/ints.o
