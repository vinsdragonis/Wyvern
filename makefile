all: bootloader

bootloader:
	mkdir boot\bin
	nasm boot/boot.asm -f bin -o boot/bin/boot.bin
	nasm boot/kernel_entry.asm -f elf -o boot/bin/kernel_entry.bin

	gcc -m32 -ffreestanding -c boot/final.c -o boot/bin/kernel.o -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -lgcc
	ld -m i386pe -o boot/bin/kernel.img -Ttext 0x1000 boot/bin/kernel_entry.bin boot/bin/kernel.o

	objcopy -O binary -j .text boot/bin/kernel.img boot/bin/kernel.bin
	type "boot\bin\boot.bin" "boot\bin\kernel.bin" > "os.img"

clear:
	rm -Force ".\boot\bin";

run:
	qemu-system-x86_64.exe -drive format=raw,file=os.img