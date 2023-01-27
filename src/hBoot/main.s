org 0x7c00
bits 16

jmp 0:start
times 5 - ($-$$) nop
BPD:
    .FSName:
        db "EXFAT   "
    .zero:
        times 53 db 0
    .part_offset:
        dd 2048
        dd 0
    .VolumeLength:
        dd 0x100000
        dd 0
    .FATOffset:
        dd 0x8000
    .FATLength:
        dd 256
    .ClusterHeapOffset:
        dd (2048 + 0x8000+ 256)
    .ClusterCount:
        dd 0x1EEE0
    .RootDirCluster:
        dd 0
    .VolSerNum:
        dd 0x20684f53
    .FsRev:
        dw 1
    .VolFlags:
        dw 0x2
    .BytesPerSectorShift:
        db 12
    .SectorsPerClusterShift:
        db 9
    .numFats:
        db 1
    .drivesel:
        db 0x80
    .percentUsed:
        db 0
    .reserved:
        times 7 db 0

start:
    
    ; jmp .set_stack
    .set_stack:
        mov bp, 0x6000
        mov sp, bp
        mov byte [bootdisc], dl
        mov bx, open
        call printstr
    .check_set_a20:
        ; jmp .load_part2
        push bx
        mov bx, 0x7dfe
        push ax
        mov ax, 0xffff
        mov es, ax
        mov di, 0x7e0e
        mov al, byte [es:di]
        cmp al, byte [bx]
        je .doublecheck
        pop ax
        pop bx
        jne .load_part2
        .doublecheck:
            sar al, 8
            mov byte [es:di], al
            mov al, byte [es:di]
            cmp al, byte [bx]
            pop ax
            pop bx
            jne .load_part2
        call enable_a20
        jc .no_a20
        jmp .check_set_a20
        .no_a20:
            or byte [bootflags], (1 << 7)
    .load_part2:
        mov bx, Part
        ; xor eax, eax
        .findloop:
            mov al, byte [bx + Part.sys_id - Part]
        ; jmp $
            cmp al, 0x7f
            je .load
            ; cmp al, 0xee
            ; je .gpt
            cmp bx, Part_3
            je error
            add bx, Part_1-Part
            jmp .findloop

        ; .gpt:
        ;     cmp cx, 34
        ;     je error
        ;     mov word [DAP.start], cx
        ;     xor ax, ax
        ;     mov ds, ax
        ;     mov si, DAP
        ;     mov ah, 0x42
        ;     mov dl, byte [bootdisc]
        ;     int 15h
        ;     mov bx, -128
        ;     .gptloop:
        ;         cmp bx, 0x200
        ;         je .gpt
        ;         add bx, 128
        ;         mov dx,  0x684f
        ;         cmp dx, word [bx + 0x800]
        ;         je .gptboot
        ;         jmp .gptloop
        ;     .gptboot:
        ;         jmp error

        .load:
            clc
            mov eax, dword [bx + Part.start_LBA-Part]
            mov word [partition], bx
            add eax, 1
            mov [p2l.start], eax
            mov ax, p2l
            mov si, ax
            xor ax, ax
            mov ds, ax
            mov ah, 0x42
            mov dl, byte [bootdisc]
            int 0x13

            jc error
            jmp part2_start
            
        jmp $
error:
    mov bx, err
    call printstr
jmp $

enable_a20:
    cmp cx, 0
    je .bios_en
    cmp cx, 1
    je .fast_a20
    stc
    ret
    .bios_en:
        pushad
        mov ax, 0x2403
        int 0x15
        jb .return
        cmp ah, 0
        jnz .return

        mov ax, 0x2402
        int 0x15
        jb .return
        cmp ah, 0
        jnz .return

        cmp al, 1
        jz start.load_part2
        
        mov ax, 0x2401
        int 0x15
        jb .return
        cmp ah, 0
        jnz .return

        jmp .return
    .fast_a20:
        pushad
        in al, 0x92
        or al, 2
        out 0x92, al
        jmp .return
    .return:
        popad
        inc cx
        ret

DAP:
    .size:
        db 0x10
    .res:
        db 0
    .sectors:
        dw 1
    .offset:
        dw 0x800
    .segemnt:
        dw 0
    .start:
        dd 0
        dd 0
p2l:
    .size:
        db 0x10
    .res:
        db 0
    .sectors:
        dw 7
    .offset:
        dw 0x7e00
    .segment:
        dw 0
    .start:
        dd 0
        dd 0

open: db "Horizon BMGR v1.0.0", 0xa, 0xd, 0
err: db "Error", 0xa, 0xd, 0
bootdisc: db 0
bootflags: db 0
booted_from_gpt: db 0
partition: dd 0
%include "src/MBR/strutils.s"
times (440) - ($-$$) db 0
db "hOS "
db 0, 0
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
        dd 2048
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

part2_start:
    mov bx, mem_dect_p
    call printstr
    detect_mem:
        mov ax, 0
        mov es, ax
        mov di, mmap_ptr
        xor ebx, ebx
        xor bp, bp
        mov edx, 0x0534d4150
        mov eax, 0xe820
        mov [es:di + 20], dword 1
        mov ecx, 24
        int 0x15
        jc mmap_failed
        mov edx, 0x534d4150
        cmp eax, edx
        jne mmap_failed
        test ebx, ebx
        je mmap_failed
        jmp .check_ent

        .loop:
            mov eax, 0xe820
            mov [es:di + 20], dword 1
            mov ecx, 24
            int 0x15
            jc .finished
            mov edx, 0x0534d4150
        .check_ent:
            jcxz .skip
            cmp cl, 20
            jbe .notext
            test byte [es:di + 20], 1
            je .skip
        .notext:
            mov ecx, [es:di + 8]
            or ecx, [es:di + 12]
            jz .skip
            inc bp
            add di, 24
        .skip:
            test ebx, ebx
            jne .loop
        .finished:
            mov [mmap_count], bp
            clc
            jmp check_hardware

check_hardware:
    mov bp, 0x2000
    mov sp, bp
    mov bx, done
    call printstr
    mov bx, hw_check_p
    call printstr
    .check_cpuid:
        pushfd
        pushfd
        xor dword [esp], 0x00200000
        popfd
        pushfd
        pop eax
        xor eax, [esp]
        popfd
        and eax, 0x00200000
        jz .check_pci2_0
        mov byte [cpuid_present], 0x1
        mov bx, cpuid_ex
        call printstr
    .check_pci2_0:
        ; xor eax, eax
        ; mov ds, eax
        ; mov es, eax
    .check_apm:
        mov ah, 0x53
        mov al, 0
        xor bx, bx
        int 0x15
        jc load_kernel
        mov al, [cpuid_present]
        or eax, 0x2
        mov byte [cpuid_present], al
        ; or byte [cpuid_present], 0x2;
load_kernel:
    clc
    mov bx, kload_p
    call printstr
    mov bx, 0
    mov ds, bx
    ; nop
    mov word [DAP2.sectors], 64
    ; cmp byte [booted_from_gpt], 0
    ; jne not_yet_supported
    mov ebx, [partition]
    mov eax, [ebx + Part.start_LBA - Part]
    add eax, 24
    mov dword [DAP2.start], eax
    ; add eax, 7
    ; jmp $
    mov word [DAP2.segemnt], 0x1000
    mov word [DAP2.offset], 0x0000
    ; mov word [DAP2.sectors], 0x1

    xor bx, bx
    xor cx, cx
    mov es, cx
    mov di, cx

    mov dl, [bootdisc]
    mov si, DAP2
    mov ah, 42h
    mov al, 0
    int 13h
    jc error
set_gdt:
    ; mov ah, 0x53
    ;     mov al, 00
    ;     xor bx, bx
    ;     int 15h
    ;     jc error
    cli
    ; ;possibly add a temporary pmode
    ; ;to load above the max
    ; ;16 bit addr mode
    lgdt [gdtinfo]
    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp 0x8:pmode_set
jmp $

not_yet_supported:
    mov bx, gpterr
    call printstr
jmp $
mmap_failed:
    mov bp, 0x2000
    mov bx, fail
    call printstr
    xor cx, cx
    xor dx, dx
    mov ax, 0xe801
    int 0x15
    cmp ah, 0x86
    je memory_err
    cmp ah, 0x80
    je memory_err
memory_err:
    mov bx, mem_err
    call printstr
jmp $

mmap_count dd 0
mmap_ptr equ 0x9000


DAP2:
    .size:
        db 0x10
    .res:
        db 0
    .sectors:
        dw 64
    .offset:
        dw 0
    .segemnt:
        dw 0x1000
    .start:
        dd 0
        dd 0

cpuid_present:
    db 0
pci2_0:
    db 0

gdtinfo:
    dw gdtend - gdts-1
    dd gdts
gdts:
    .null:
        dd 0
        dd 0
    .kcode:
        dw 0xffff
        dw 0
        db 0
        db 0b10011010
        db 0b11001111
        db 0
    .kdata:
        dw 0xffff
        dw 0
        db 0
        db 0b10010010
        db 0b11001111
        db 0
gdtend:
data:
mem_dect_p: db "Obtaining memory map: ", 0
hw_check_p: db "Checking Hardware", 0xa, 0xd, 0
mem_err: db "FATAL: COULD NOT DETECT ANY MEMORY", 0xa, 0xd, "UNSAFE TO CONTINUE... HALTING", 0Xa, 0xd, 0
done: db "Done", 0xa, 0xd, 0
fail: db "Failed", 0xa, 0xd, 0
cpuid_ex: db "Found: CPUID", 0xa, 0xd, 0
kload_p: db "Loading Kernel", 0xa, 0xd, 0
gpterr: db "GPT Loading not currently supported", 0xa, 0xd, 0

[bits 32]
pmode_set:
    mov eax, 0x10
    mov ds, eax
    mov ss, eax
    mov gs, eax
    mov fs, eax
    mov es, eax
    ; jmp $
    push dword [mmap_count]
    push dword mmap_ptr
    push byte cpuid_present     
    jmp 0x8:0x10000
    ; db "Hello, World", 0
jmp pmode_set
times 4608 - ($-$$) db 0
times 1024 db 0
main_checksum:
times 512 db 0
backup:
times 12 * 512 db 0
