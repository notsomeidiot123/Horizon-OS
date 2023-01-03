org 0x7c00
bits 16

jmp start
times 3 - ($-$$) nop
BPD:
    .oem:  ;12345678
        db "Horiz_OS"
    .bytesPerSector:
        dw 0x200
    .sectors_per_cluster:
        db 8
    .reserved_sectors:
        dw 0
    .fat_count:
        db 0
    .root_dir_entries:
        dw 0
    .total_sectors:
        dw 0
    .media_descriptor:
        db 0xf8
    .num_sectors_per_fat_unused:
        dw 0
    .sectors_per_track:
        dw 0
    .num_heads:
        dw 0
    .hidden_sectors:
        dd 0
    .large_sector_count:
        dd 0
EBPB:
    .sectors_per_fat:
        dd 0
    .flags:
        dw 0
    .FAT_version:
        db 1, 0
    .cluster_number_root_dir:
        dd 0
    .sector_number_FSINFO:
        dw 0
    .sector_number_backup_boot:
        dw 0
    .reserved:
        dd 0
        dd 0
        dd 0
    .drive_no:
        db 0
    .flags_NT:
        db 0
    .sig:
        db 0x28
    .serial_ID:
        dd 0
    .label:
           ;12345678901
        db "MAIN:      "
    .sys_id:
        db "FAT32   "


start:
    mov byte [bootdisc], dl
    mov bx, open
    call printstr
    jmp .set_stack
    .set_stack:
        mov bp, 0x1000
        mov sp, bp
        xor cx, cx
        xor dx, dx
    .check_set_a20:
        ; jmp .load_part2
        mov ax, 0xffff
        mov es, ax
        xor ax, ax
        
        mov si, 0x7e0e
        
        mov ax, word [es:si]
        
        cmp ax, 0xaa55
        
        jne .load_part2
        call enable_a20
        jc .no_a20
        jmp .check_set_a20
        .no_a20:
            or byte [bootflags], (1 << 7)
    .load_part2:
        mov bx, Part.sys_id
        ; xor eax, eax
        mov al, byte [bx]
        ; jmp $
        cmp al, 0x7f
        je .load
        cmp al, 0xee
        je .gpt
        cmp bx, Part_3.sys_id
        je error

        .gpt:

        .load:

        jmp $
error:
    mov bx, err
    call printstr
jmp $

enable_a20:
    cmp cx, 0
    je .bios_en
    cmp cx, 1
    je .kb_en
    cmp cx, 2
    je .fast_a20
    stc
    ret
    .bios_en:
        jmp .return
    .kb_en:
        jmp .return
    .fast_a20:
        jmp .return
    .return:
        inc cx
        ret

open: db "Horizon Boot Loader v1.0.0", 0xa, 0xd, 0
err: db "Error", 0xa, 0xd, 0
bootdisc: db 0
bootflags: db 0
%include "src/MBR/strutils.s"
times (440) - ($-$$) db 0
Part:
    .flags:
        db 0x80;bootable offset 0
    .start_head:
        db 0
    .start_sector:
        db 2
    .start_cyl:
        db 1
    .sys_id:
        db 0x7f
    .end_head:
        db 0xff
    .end_sector:
        db 0xff
    .end_cyl:
        db 0xff
    .start_LBA:
        dd 1
    .size:
        dd 0xffffFFFF
Part_1:
    .flags:
        db 0;bootable offset 0
    .start_head:
        db 0
    .start_sector:
        db 0
    .start_cyl:
        db 0
    .sys_id:
        db 0
    .end_head:
        db 0
    .end_sector:
        db 0
    .end_cyl:
        db 0
    .start_LBA:
        dd 0
    .size:
        dd 0
Part_2:
    .flags:
        db 0;bootable offset 0
    .start_head:
        db 0
    .start_sector:
        db 0
    .start_cyl:
        db 0
    .sys_id:
        db 0
    .end_head:
        db 0
    .end_sector:
        db 0
    .end_cyl:
        db 0
    .start_LBA:
        dd 0
    .size:
        dd 0
Part_3:
    .flags:
        db 0;bootable offset 0
    .start_head:
        db 0
    .start_sector:
        db 0
    .start_cyl:
        db 0
    .sys_id:
        db 0
    .end_head:
        db 0
    .end_sector:
        db 0
    .end_cyl:
        db 0
    .start_LBA:
        dd 0
    .size:
        dd 0
times 510 - ($-$$) db 0
db 0x55, 0xaa

times 4096 - ($-$$) db 0