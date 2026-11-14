IMG ?= ../malux005.img

FINISH: cLib
	@dd if=cLib of=$(IMG) bs=512 seek=69 count=2 conv=notrunc status=none
cLib: Chars.asm
	@nasm Chars.asm -f bin cLib
