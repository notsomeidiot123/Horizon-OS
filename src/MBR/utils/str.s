printstr:
    mov ah, 0xe
    mov si, bx
    .strloop:
        lodsb
        or al, al
        jz .printstr_ret
        int 0x10
        jmp .strloop
    .printstr_ret:
        ret

