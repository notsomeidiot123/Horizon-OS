bits 32
    ; jmp $
    mov byte [0xb8000], 0x41

jmp $
times 4096 - ($-$$) db 0