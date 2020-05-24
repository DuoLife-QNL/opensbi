qemupath:= /home/halloween/Documents/Gitee/DuoLife-QNL/qemu/build/riscv64-softmmu/
opensbipath := /home/halloween/Documents/Github/opensbi/

CC 	:= riscv64-unknown-elf-gcc -mcmodel=medany -O0
LD	:= riscv64-unknown-elf-ld -T llinker.ld
AS	:= riscv64-unknown-elf-as
objcopy := riscv64-unknown-elf-objcopy

test_main.o : test_main.c
	$(CC) -c test_main.c -o test_main.o

entry.o	: entry.asm
	$(AS) entry.asm -o entry.o
test.elf:entry.o test_main.o
	$(LD) entry.o test_main.o -o test.elf
bin: test.elf
	$(objcopy) test.elf --strip-all -O binary test.bin
qemu:
	$(qemupath)qemu-system-riscv64 \
        -machine virt \
        -nographic \
	-trace events=./events \
	-bios $(opensbipath)build/platform/generic/firmware/fw_jump.bin \
	-device loader,file=test.bin,addr=0x80200000 \
	-kernel $(opensbipath)build/platform/generic/firmware/fw_payload.bin

analyze:trace-*
	$(qemupath)../../scripts/simpletrace.py trace-events trace-*


