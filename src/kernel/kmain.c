#include "memory/mmanager.h"
#include "vga/graphics.h"
#include "drivers/idt.h"
#include "drivers/devices.h"

extern unsigned int ksize;
extern void kmain(unsigned char flags, mmap_ent *mmap_ptr, unsigned int mmap_count ){
    setTheme(0, TM_GREEN);
    vga_desc.textmode = 1;
    kclear_text();
    t_en_cursor(0, 0);
    kprintf("Starting HorizonOS 0.1.0-32\n");
    // kputs("Hello, World! \nh");
    kprintf("Mmap Entries: %d\n", mmap_count);
    kprintf("Mmap Pointer: 0x%x\n", mmap_ptr);
    kprintf("Initializing RAMAT ");
    unsigned int total_usable_ram = initram(mmap_count, mmap_ptr);
    kprintf(total_usable_ram ? "[Success] : %dMB Free\n" : "[Failed]\n", total_usable_ram/1024/1024);
    void *rdsp_loc = locate_rsdp(parse_bda());
    kprintf("Locating RDSP: %s\n", rdsp_loc ? "[FOUND]" : "[NOT FOUND]");
    
    // kprintf("Starting PS/2 Keyboard %s\n", init_ps_2_kbd(0)? "[Success]" : "[Failed]");
    kprintf("Initializing IDT ");
    idt_init();
    kprintf("[Finished]\nInstalling IRQ Handlers ");
    irq_install_handler(1, ps2kbd_irq);
    irq_install_handler(0, pit_tick_handler);
    pic_mask_master.data.ps2kb = 1;
    pic_mask_master.data.pit = 1;
    pic_remask(pic_mask_master.byte, pic_mask_slave.byte);
    
    kprintf("[Success]\n");
    
    unsigned int cpuid_res = get_cpuid(flags);
    kprintf("Getting CPUID: %s\n",cpuid_res ? procinfo.vendor : "[Unavailable]");
    if(procinfo.dword_0.strct.virt){
        kprintf("VM Detected!\n");
    }
    if(procinfo.dword_1.strct.acpi){
        kprintf("[ACPI Detected]");
    }
    kprintf("%s%s\n", procinfo.dword_1.strct.pae ? "[PAE Available] " : "", procinfo.dword_1.strct.apic ? "[APIC Available]" : "");
    
    for(;;);
    // return;
}