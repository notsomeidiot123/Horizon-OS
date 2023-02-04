nasm -f bin src/MBR/kiminoMBR.s -o bin/mbr/mbr.bin
nasm -f bin src/hBoot/main.s -o bin/boot/boot.bin
nasm -f elf src/kernel/kentry.s -o bin/kernel/kentry.o
nasm -f elf src/kernel/drivers/idt.s -o bin/kernel/idt.o
gcc src/kernel/kmain.c -o bin/kernel/kernel.o -ffreestanding -c -g -fno-pie\
 -m32 -O0 -Wno-int-to-pointer-cast -Wno-int-to-pointer-cast -Wno-incompatible-pointer-types\
  -Wno-packed-not-aligned -Wno-implicit
g++ src/kernel/shell/shell.cpp -c -g -fno-pie\
 -m32 -O0 -o bin/shell/shell.o
ld -T linker.ld -melf_i386
cat bin/mbr/mbr.bin  > HorizonOS.iso
qemu-img resize HorizonOS.iso 1M
cat bin/boot/boot.bin bin/kernel/kernel.bin >> HorizonOS.iso
qemu-img resize HorizonOS.iso 512M
sudo parted HorizonOS.iso mkpart primary 1M 100%
hd HorizonOS.iso > hd.txt

./run