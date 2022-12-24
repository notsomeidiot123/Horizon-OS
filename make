nasm -f bin src/MBR/main.s -o bin/mbr/mbr.bin

cat bin/mbr/mbr.bin > HorizonOS.iso

hd HorizonOS.iso > hd.txt
# qemu-img resize HorizonOS.iso 512M

./run