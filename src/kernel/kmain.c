#include "memory/linear.h"
#include "vga/graphics.h"
#include "drivers/idt.h"
#include "drivers/input/ps2.h"
extern void kmain(unsigned char flags, mmap_ent *mmap_ptr, unsigned int mmap_count ){
    setTheme(0, TM_GREEN);
    kclear_text();
    t_en_cursor(0, 0);
    kprintf("Starting HorizonOS 0.1.0-32\n");
    // kputs("Hello, World! \nh");
    kprintf("Mmap Entries: %d\n", mmap_count);
    kprintf("Mmap Pointer: %x\n", mmap_ptr);
    kprintf("Initializing RAMAT ");
    unsigned int total_usable_ram = initram(mmap_count, mmap_ptr);
    kprintf(total_usable_ram ? "[Success] : %dMB Free\n" : "[Failed]\n", total_usable_ram/1024/1024);
    
    kprintf("Initializing IDT ");
    idt_init();
    kprintf("[Finished]\n");
    
    irq_install_handler(1, ps2kbd_irq);
    kprintf("%x", interrupt_handlers[1]);
    for(;;);
    // return;
}