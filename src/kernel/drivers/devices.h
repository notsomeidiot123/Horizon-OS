#pragma once
#include "input/ps2.h"
#include "idt.h"
#include <cpuid.h>

#define KB_PS2 0
#define KB_USB 1
#define KEYMAP_EN_US_ASCII 0

struct{
    struct{
        unsigned char pata_0:1;
        unsigned char pata_1:1;
        unsigned char pata_2:1;
        unsigned char pata_3:1;
    }data_storage;
    
    struct{
        unsigned char apic:1;
        unsigned char cores:7;
    }cpu;
    
    struct{
        unsigned char pcspeaker:1;
    }audio;
    
    struct 
    {
        unsigned char ps_2_kbd : 1;
        unsigned char ps_2_kbd_keymap : 2;
        unsigned char ps_2_mouse : 1;
    }ps_2input;
    
    //USB, COM & others
    struct{
        
    }external;
    
    struct{
        
    }internet;
    
    
}connected;


struct{
    unsigned char type : 1;
    // unsigned char 
    unsigned int keymap;
    unsigned char *stdin;
    unsigned char *stdout;
    void *driver;
}keyboard_desc;

struct{
    union{
        unsigned int byte;
        struct{
            unsigned int sse3: 1;
            unsigned int pclmulqdq : 1;
            unsigned int dtes64 : 1;
            unsigned int monitor: 1;
            unsigned int ds_cpl : 1;
            unsigned int vmx : 1;
            unsigned int smx : 1;
            unsigned int est : 1;
            unsigned int tm2 : 1;
            unsigned int ssse3 : 1;
            unsigned int cnxt_id : 1;
            unsigned int sdbg : 1;
            unsigned int fma : 1;
            unsigned int cx16 : 1;
            unsigned int xtpr : 1;
            unsigned int pdcm : 1;
            unsigned int res0 : 1;
            unsigned int pcid : 1;
            unsigned int dca : 1;
            unsigned int sse4_1: 1;
            unsigned int sse4_2: 1;
            unsigned int x2apic : 1;
            unsigned int movbe : 1;
            unsigned int popcnt : 1;
            unsigned int tsc_deadline : 1;
            unsigned int aes : 1;
            unsigned int xsave : 1; 
            unsigned int osxsave : 1;
            unsigned int avx : 1;
            unsigned int f16c : 1;
            unsigned int rdrnd : 1;
            unsigned int virt : 1;
        }strct;
    }dword_0;
    union{
        unsigned int byte;
        struct{
            int fpu: 1;
            int vme: 1;
            int de: 1;
            int pse : 1;
            int tsc : 1;
            int msr : 1;
            int pae : 1;
            int mce : 1;
            int cx8 : 1;
            int apic : 1;
            int res0 : 1;
            int sep : 1;
            int mtrr : 1;
            int pge : 1;
            int mca: 1;
            int cmov : 1;
            int pat : 1;
            int pse_36 : 1;
            int psn : 1;
            int clfsh : 1;
            int res1 : 1;
            int ds : 1;
            int acpi : 1;
            int mmx : 1;
            int fxsr : 1;
            int sse : 1;
            int sse2 : 1;
            int ss : 1;
            int htt : 1;
            int tm : 1;
            int ia64 : 1;
            int pbe : 1;
        }strct;
    }dword_1;
    char vendor[13];
}__attribute__((packed))procinfo;

int get_cpuid(unsigned char cpuid_present){
    if((cpuid_present&1) == 0){
        return -1;
    }
    unsigned int eax, ecx, edx, ebx = 0;
    __cpuid(0, eax, ebx, ecx, edx);
    memcpy((char *)&ebx, procinfo.vendor, 4);
    memcpy((char *)&edx, procinfo.vendor+4, 4);
    memcpy((char *)&ecx, procinfo.vendor+8, 4);
    procinfo.vendor[12] = 0;
    if(eax == 0 || !ecx && !edx && !ebx){
        return -1;
    }
    __cpuid(1, eax, ebx, ecx, edx);
    procinfo.dword_0.byte = ecx;
    procinfo.dword_1.byte = edx;
    // for(;;);
}
struct bda {
    unsigned short io_com[4];
    unsigned short io_lpt[3];
    unsigned short EBDA_base;
    unsigned short hardware;
    unsigned short kb_state;
    unsigned char kb_buffer[32];
    unsigned char display_mode;
    unsigned short textmode_max_x_res;
    unsigned short video_io_base;
    unsigned short ticks_since_boot;
    unsigned char num_hard_drives;
    unsigned short kb_buffer_start;
    unsigned short kb_buffer_end;
    unsigned char kd_led_state;

} *bios_data_area;
char * parse_bda(){
    bios_data_area = (struct bda*)0x400;
    kprintf("[Debug] EBDA Located: [0x%x]\n", (bios_data_area->EBDA_base) << 4);
    kprintf("[Debug] Display mode: [%d]\n", bios_data_area->display_mode);
    // kprintf("[Debug] ");
    return bios_data_area->EBDA_base << 4;
}
struct RSDPointer{
    char Signature[8];
    unsigned char Checksum;
    char OEMID[6];
    unsigned char Revision;
    unsigned int RsdtAddress;
}__attribute__((packed)) *RSDP;
struct RSDPointer *locate_rsdp(char *ebda){
    for(int i = ebda; i < ebda + 0x400; i += sizeof(struct RSDPointer)){
        RSDP = i;
        if(strcmp(RSDP->Signature, "RDS PTR")){
            return RSDP;
        }
    }
    return NULL;
}
unsigned int ticks = 0;
unsigned int seconds = 0;



void pit_tick_handler(registers *regs){
    ticks++;
    if((ticks % 18) == 0){
        seconds++;
    }
    
}
int wait_seconds(unsigned int secs){
    static unsigned int future;
    if(secs == 0){
        return seconds >= future;
    }else{
        future = secs;
        
    }
}
int wait_ticks(unsigned int fticks){
    static unsigned int future;
    if(ticks == 0){
        return ticks >= future;
    }else{
        future = fticks;
    }
}