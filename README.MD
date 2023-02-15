<div align="center"><h1>HorizonOS</h1></div>
<div align="center"><img src="https://img.shields.io/static/v1?label=Version&message=0.1.0&color=green&style=flat-square"></img>&nbsp&nbsp<img src="https://img.shields.io/static/v1?label=Status&message=Stable&color=green&style=flat-square"</img>&nbsp&nbsp<img src="https://img.shields.io/static/v1?label=Contributors&message=1&color=green&style=flat-square"></img></div>

# Credits
## Contriubutors 
Kimi | [notsomeidiot123](https://github.com/notsomeidiot123/) - Creator

## Resources used

[OSDev Wiki](https://osdev.org)

[Wikipedia - BIOS Interrupt calls](en.wikipedia.org/wiki/BIOS_iterrupt_calls)

[HDDGuru - ATA-ATAPI-standard-6](https://hddguru.com/download/documentation/ATA-ATAPI-standard-6/ATA-ATAPI-6.pdf)

[Microsoft - IDENTIFY_DEVICE_DATA structure (ata.h)](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ata/ns-ata-_identify_device_data?redirectedfrom=MSDN)
# Project

[Changelog](info/changelog.md)

## Introduction

Wecome to the home of the (new) Operating System that I am working on. Horizon is an open-source operating system, less focused on compatibility with older software, and more of a general-purpose operating system. You can read more about the smaller details [here](info/plans.md)

## Details

Right now, we are working on developing the bare bones kernel.

HorizonOS uses the ~~exFAT~~ FAT32 (2023-02-14) filesystem, but planning to support [HAT32](info/HAT32.md)

### Compilation

HorizonOS's default build files require GCC, LD, and NASM, and all the code written is written around how these compilers work, and is *very* platform dependent. I (for some reason) have not been able to get gcc to properly compile my operating system on Arch (i use arch btw lmao), but on both ParrotOS, and Ubuntu, it has been able to work just fine. Also, some of the test hardware that is being emulated, by default, is not included in the qemu-full package on Arch, so you may have to remove those lines during emulation. 

For these reasons, it is reccomended to directly download from the [releases](https://github.com/notsomeidiot123/Horizon-OS/releases) tab. if that is not possible (but downloading the entire repository is), it is recommended to then compile it on a debian-based system(?). If that is not possible, then either give up, or try to mangle and force your compiler to compiling it correctly. Correct output not guarunteed. Also, when compiling, please use as many of the pre-defined switches in either the make or make-arch build files, as many of them are required in order for it to compile to a freestanding enviornment. Later on, I may set up a cross-compiler (along with the proper setup scripts), however, until then, this is what we have.

### Why do all these build/run/whatever scripts suck so bad?

To be completely honest, I am a complete noob when it comes to *nix, so even basic things (like getting VNC to work - that took forever and is still janky lmao) will take a while. Anything that has to do with my code for my operating system, (the .c, .h, .s source files) I know like the back of my hand, so please if you find an issue with it, report it ASAP, and I will get it fixed within the next couple of commits
