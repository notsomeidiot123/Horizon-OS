# nasm -f bin src/MBR/kiminoMBR.s -o bin/mbr/mbr.bin
nasm -f bin src/hBoot/main.s -o bin/boot/boot.bin
nasm -f elf src/kernel/kentry.s -o bin/kernel/kentry.o

ld -T linker.ld -melf_i386
cat bin/mbr/mbr.bin bin/boot/boot.bin bin/kernel/kernel.bin > HorizonOS.iso

hd HorizonOS.iso > hd.txt
# qemu-img resize HorizonOS.iso 512M

./run