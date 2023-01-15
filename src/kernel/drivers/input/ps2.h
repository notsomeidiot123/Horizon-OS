#include "../idt.h"
#include "../../vga/graphics.h"
void ps2kbd_irq(registers *r){
    kprintf("KeyCode: %x", inb(0x60));
    return;
}