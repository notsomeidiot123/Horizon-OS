#pragma once

#include "idt.h"
#include "../memory/mmanager.h"
#include <cpuid.h>
#include "acpi.h"
#include "disks/ata.h"

#define KB_PS2 0
#define KB_USB 1
#define KEYMAP_EN_US_ASCII 0

#ifdef DEBUG
    int lookfor_linux = 1;
#else  
    int lookfor_linux = 0;
#endif

#define DRIVE_SUCCESS 0
#define DRIVE_ERR 0x80
#define NO_EXIST + 1
#define OUT_BOUNDS + 2
#define READ_ERR + 3
#define WRITE_ERR  + 4


enum storage_driver_type{
    NULL_DRIVER,
    ATA_PIO28,
    ATA_DMA28,
    ATA_PIO48,
    ATA_DMA48,
    SATA_LEGACY,
    SATA_ACHI,
    ATAPI
};


struct{
    struct{
        struct{
            unsigned int max_lba_low;
            unsigned int max_lba_hih;
            enum storage_driver_type;
            unsigned char bootable:1;
            unsigned char reserved:1;
            unsigned char connected:1;
            unsigned char mount: 5;
            char name[32];
        }drives[32];
        unsigned char ata_last_selected;
        unsigned char acpi;
    }data_storage;
    
    struct{
        unsigned char apic:1;
        unsigned char cores:7;
    }cpu;
    
    struct{
        unsigned char pcspeaker:1;
        unsigned char sb16:1;
    }audio;
    
    struct 
    {
        unsigned char ps_2_kbd : 1;
        unsigned char ps_2_kbd_keymap : 2;
        unsigned char ps_2_mouse : 1;
        unsigned char kdb_controller : 1;
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

int cmprsdt(char *ptr){
    char *rdststr = "RSD PTR ";
    for(int i = 0; i < 8; i++){
        if(ptr[i] == rdststr[i]){
            continue;
        }else{
            return 0;
        }
    }
    return 1;
}
struct RSDPointer *locate_rsdp(char *ebda){
    struct RSDPointer *tmp = NULL;
    for(int i = ebda; i < ebda + 0x1000; i += sizeof(RSDP)){
        RSDP = i;
        if(cmprsdt(RSDP->Signature)){
            tmp = RSDP;
            break;
        }
    }
    if(tmp == NULL){
        for(int i = 0xe0000; i < 0xfffff; i+= sizeof(RSDP)){
            RSDP = i;
            if(cmprsdt(RSDP->Signature)){
                tmp = RSDP;
                break;
            }
        }
    }
    // kprintf("%x", (unsigned )RSDP);
    // struct ACPISDTHeader *header = tmp->RsdtAddress;
    // kprintf("%x", header);
    if(!(cmprsdt(RSDP->Signature))){
        connected.data_storage.acpi = 0;
        connected.ps_2input.kdb_controller = 1; //assumed. if no ACPI
        return 0;
    }
    if(RSDP->Revision == 1){
        kprintf("ACPI 2.0");
        acpi_version = 1;
    }else{
        connected.ps_2input.kdb_controller = 1;
    }
    connected.data_storage.acpi = 1;
    return RSDP;
}
char table_types[][5] = {
    "NULL\0",
    "APIC\0",
    "DSDT\0",
    "FACP\0",
    "SRAT\0",
    "SSDT\0",
    "XSDT\0",
    "RSDT\0",
    "HPET\0",
    "UNKN\0",
};

int check_table_type(char sig[4]){
    
    int nomat = 0;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 4; j++){
            if(sig[j] != table_types[i][j]){
                nomat = 1;
                break;
            }
        }
        if(!nomat){
            return i;
        }else{
            nomat = 0;
        }
    }
    return 0;
}

int parse_fadt(struct ACPISDTHeader *header){
    if(header == NULL){
        return 1;
    }
    FADT_PRESERVED = header;
    // kprintf("Test");
    if(!(FADT_PRESERVED->BootArchitectureFlags & 2) && acpi_version){
        connected.ps_2input.kdb_controller = 0;
    }else if(FADT_PRESERVED->BootArchitectureFlags & 2 && acpi_version){
        connected.ps_2input.kdb_controller = 1;
    }
    kprintf("[Finished]\n");
    int res = parseDSDT(FADT_PRESERVED->Dsdt);
    return 0;
    //parse DSDT
}

void parse_rdst(struct RSDPointer *rsdp){
    if(rsdp == NULL){
        kprintf("ACPI ERROR 0x01\n");
        return;
    }
    struct ACPISDTHeader *header = (struct ACPISDTHeader *)rsdp->RsdtAddress;
    enum ACPI_TABLE_TYPE type = check_table_type(header->Sig);
    unsigned int len = (header->len - sizeof(*header))/4;
    unsigned int *sdt_pointers = header;
    sdt_pointers += (36/4);
    //IDK WHY THIS DOESNT WORK ON THE LINE LITERALLY RIGHT ABOVE IT
    
    kprintf("Entries: %d\n", len);
    for(int i = 0; i < len; i++){
        struct ACPISDTHeader *h = (struct ACPISDTHeader*)sdt_pointers[i];
        enum ACPI_TABLE_TYPE t_type = check_table_type(h->Sig);
        kprintf("|-");
        switch(t_type){
            case ACPI_FADT:
                kprintf("Parsing FADT: ");
                if(parse_fadt(h)){
                    kprintf("\r|-FADT: ACPI ERROR 0x03\n");
                    continue;
                }
                break;
            case ACPI_MADT:
                kprintf("Parsing MADT: ");
                break;
            case ACPI_NULL | ACPI_UNKN:
                kprintf("ACPI ERROR 0x02\n");
                continue;
                break;
                // return;
            default:
                kprintf("%s: ACPI ERROR 0x4\n", table_types[t_type]);
                continue;
        }
        kprintf("[Finished]\n");
    }
    kprintf("|______________________\n");
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
        // if(seconds >= future){
        //     kprintf("Finished timer\n");
        // }
        return seconds < future;
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
#include "input/ps2.h"