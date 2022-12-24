[org 0x600]
bits 16
reloc:
    mov sp, 0x1000
    mov bp, sp
    
    ; push dx

    mov byte [bootdisc], dl

    mov ax, 0
    mov ds, ax
    mov es, ax
    mov si, 0x7c00
    mov di, 0x600
    mov cx, 256
    rep movsw

    jmp 0x0:power_on

power_on:
    
count_active:
    mov cx, 1
    xor dx, dx
    mov bx, part0
    .start_loop:
        cmp byte [bx], 0x80
        jb .next
        
        or [active_struct], cx
        
        shl cx, 1
        inc dx
        nop
    .next:
        cmp bx, part3
        je boot_if_one_only

        add bx, [part_size]

        jmp .start_loop

boot_if_one_only:
    cmp dx, 0
    je no_part
    cmp dx, 1
    jne select_and_boot
    jmp $

    push dx
    xor dx, dx
    mov cx, [active_struct]
    mov ax, 2
    div cx
    pop dx
    mov cx, ax
    jmp select_and_boot.read_chs
select_and_boot:
    mov ah, 0xe 
    mov al, dl
    add al, 0x30
    int 10h

    mov bx, multi
    call printstr
    .invalid:
    mov bx, sel
    call printstr

    .select:
        xor ax, ax
        int 16h
        mov ah, 0xe
        int 10h
        sub al, 48
        cmp al, 4
        jg .select
        cmp al, 0
        jle .select
    .read_start:
        mov bx, part0
        sub ax, 1
        mul byte [part_size]
        add bx, ax
        cmp byte [bx], 0x80
        jnb .read_chs
        .inv:
        mov bx, invld
        call printstr
        jmp .invalid
    .read_chs:
        mov al, byte [bx + part0.sector-part0]
        and al, 0x3f
        cmp al, 0
        je inv_sec
        xor ax, ax
        mov es, ax

        mov ah, 2
        mov al, 1
        mov ch, byte [bx+part0.cyl - part0]
        mov cl, byte [bx+part0.sector - part0]
        mov dh, byte [bx+part0.start_head - part0]
        mov dl, byte [bootdisc]
        mov bx, 0x7c00
        int 13h

        cmp ah, 0
        jne read_err_f

        jmp bx
        jmp $
inv_sec:
    call error
    mov ah, 0xe
    mov al, 49
    int 10h
    mov bx, invld_sec
    call printstr
    call endline
    jmp $
read_err_f:
    push ax
    
    call error
    mov ah, 0xe
    mov al, 50
    int 10h
    mov bx, read_err
    call printstr
    pop ax
    mov al, ah
    mov ah, 0xe
    add al, 48
    int 10h
    call endline
    jmp $

no_part:
    mov bx, none_act
    call printstr
jmp $

error:
    mov bx, err
    call printstr
    ret

endline:
    mov bx, endl
    call printstr
    ret

%include "src/MBR/utils/str.s"

multi: db " Partitons found", 0xa, 0xd, 0
sel: db "Please Select Which To Boot", 0xa, 0xd, 0x0
none_act: db "No bootable partition found!", 0xa, 0xd, 0
invld: db 0xa, 0xd, "Invalid Partition", 0xa, 0xd, 0
invld_sec: db ": INV_SEC", 0xa, 0xd, 0
err: db 0xa, 0xd, "Err ", 0
read_err: db ": READ_ERR "
endl: db 0xa, 0xd, 0
bootdisc: db 0
part_size: dw part1-part0

active_struct: db 0


times 440-($-$$) db 0

db "hOS "
dw 0

part0:
    .flags:
        db 0x80
    .start_head:
        db 0
    .sector:
        db 0x41
    .cyl:
        db 0
    .sys_id:
        db 0
    .end_head:
        db 0
    .end_sector:
        db 0
    .end_cyl:
        db 0
    .relative_sector:
        dd 0
    .total_sectors:
        dd 0
part1:
    .flags:
        db 0x80
    .start_head:
        db 0
    .sector:
        db 0
    .cyl:
        db 0
    .sys_id:
        db 0
    .end_head:
        db 0
    .end_sector:
        db 0
    .end_cyl:
        db 0
    .relative_sector:
        dd 0
    .total_sectors:
        dd 0
part2:
    .flags:
        db 0
    .start_head:
        db 0
    .sector:
        db 0
    .cyl:
        db 0
    .sys_id:
        db 0
    .end_head:
        db 0
    .end_sector:
        db 0
    .end_cyl:
        db 0
    .relative_sector:
        dd 0
    .total_sectors:
        dd 0
part3:
    .flags:
        db 0
    .start_head:
        db 0
    .sector:
        db 0
    .cyl:
        db 0
    .sys_id:
        db 0
    .end_head:
        db 0
    .end_sector:
        db 0
    .end_cyl:
        db 0
    .relative_sector:
        dd 0
    .total_sectors:
        dd 0
db 0x55, 0xaa
times 512-($-$$) db 0