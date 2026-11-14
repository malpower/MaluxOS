IMG ?= ../malux005.img

BOOTER: booterHDD booterFDD
	dd if=booterHDD of=$(IMG) bs=512 seek=0 count=1 conv=notrunc status=none
booterHDD: booterHDD.asm
	nasm -f bin booterHDD.asm -o booterHDD
