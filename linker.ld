/* ENTRY(bin/kernel/kentry.o) */
STARTUP(bin/kernel/kentry.o)
OUTPUT_FORMAT(binary)
INPUT(
    bin/kernel/kentry.o
    bin/kernel/kernel.o
    bin/kernel/idt.o
)


SECTIONS{
    .text.start (0x10000) : {
        bin/kernel/kentry.o( .text )
        bin/kernel/kernel.o( .text )
    }
    .text : ALIGN(CONSTANT(MAXPAGESIZE)) {
        _TEXT_START_ = .;
        *(.text)
        _TEXT_END_ = .;
    }
    .data : ALIGN(CONSTANT(MAXPAGESIZE)) {
        _DATA_START_ = .;
        *(.data)
        _DATA_END_ = .;
    }
    .bss : ALIGN(CONSTANT(MAXPAGESIZE)) {
        _BSS_START_ = .;
        *(.bss)
        _BSS_END_ = .;
    }
    
}
ksize = . + 1;
OUTPUT(bin/kernel/kernel.bin)