#include "memory/mmanager.h"
#include "vga/graphics.h"
#include "drivers/idt.h"
#include "drivers/devices.h"
unsigned int noacpi_debug = 1;
extern unsigned int ksize;
extern void kmain(unsigned char flags, mmap_ent *mmap_ptr, unsigned int mmap_count ){
    setTheme(0, LIGHT TM_LGRAY);
    vga_desc.textmode = 1;
    
    kclear_text();
    t_en_cursor(0, 0);
    kprintf("Starting HorizonOS 0.1.0-32\n");
    kprintf("Initializing IDT ");
    idt_init();
    pic_disable();
    kprintf("[Finished]\nInstalling Tick Handler ");
    // irq_install_handler(1, ps2kbd_irq);
    irq_install_handler(0, pit_tick_handler);
    pic_mask_master.data.pit = 1;
    pic_remask(pic_mask_master.byte, pic_mask_slave.byte);
    kprintf("Getting Memory Information...\n");
    kprintf("Mmap Entries: %d\n", mmap_count);
    kprintf("Mmap Pointer: 0x%x\n", mmap_ptr);
    kprintf("Initializing RAMAT ");
    unsigned int total_usable_ram = initram(mmap_count, mmap_ptr);
    kprintf(total_usable_ram ? "[Success] : %dMB Free\n" : "[Failed]\n", total_usable_ram/1024/1024);
    void *rdsp_loc = locate_rsdp(parse_bda());
    kprintf("Locating RDSP: %s\n", rdsp_loc ? "[FOUND]" : "[NOT FOUND]");
    if(rdsp_loc != NULL){
        kprintf("Parsing RSDP: \n");
        parse_rdst(rdsp_loc);
    }
    
    if(connected.ps_2input.kdb_controller || connected.data_storage.acpi == 0 || noacpi_debug){
        unsigned int kbd_res = init_ps_2_kbd(0, horizonShellTextInput);
        kprintf("Starting 8042 Controller: %s\n", kbd_res? "[Success]" : "[Failed]");
        if(kbd_res){
            irq_install_handler(1, ps2kbd_irq);
        }
    }
    unsigned int cpuid_res = get_cpuid(flags);
    kprintf("Getting CPUID: %s\n",cpuid_res ? procinfo.vendor : "[Unavailable]");
    if(procinfo.dword_0.strct.virt){
        kprintf("VM Detected!\n");
    }
    if(procinfo.dword_1.strct.acpi){
        kprintf("[ACPI Detected]");
    }
    kprintf("%s%s\n", procinfo.dword_1.strct.pae ? "[PAE Available] " : "", procinfo.dword_1.strct.apic ? "[APIC Available]" : "");
    kprintf("Pausing for Readability...\n");
    wait_seconds(3);
    while(wait_seconds(NULL));
    
    
    enable_ps2kbd();
    // pic_disable();
    horizonShell();
    for(;;);
    // return;
}