echo "Creating Directories"
mkdir bin
mkdir testDrives
cd testDrives/
echo "Creating Drives in:";pwd
touch fdrive0.fd
qemu-img resize fdrive0.fda 720K
touch hdrive1.hd
qemu-img resize hdrive1.hda 512M
ls -lha
cd ../
echo "Creating bin subdirectories"
cd bin
pwd
mkdir mbr
mkdir boot
mkdir kernel
mkdir shell
ls -lha
cd ../
pwd
echo "Changing file mode of make and run"
chmod 777 ./make
chmod 777 ./run
echo "Finished"