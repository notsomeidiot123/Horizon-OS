nasm -f bin src/MBR/kiminoMBR.s -o bin/mbr/mbr.bin
nasm -f bin src/hBoot/main.s -o bin/boot/boot.bin
cat bin/mbr/mbr.bin bin/boot/boot.bin > HorizonOS.iso

hd HorizonOS.iso > hd.txt
# qemu-img resize HorizonOS.iso 512M

./run