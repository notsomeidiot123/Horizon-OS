extern kmain

bits 32
    ; jmp $
    ; mov word [0xb8000], 0x0f42
    call kmain
jmp $