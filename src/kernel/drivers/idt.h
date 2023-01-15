#include "../memory/memory.h"
#include "../vga/graphics.h"
#pragma once

typedef struct
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} registers;

typedef struct {
    unsigned short BaseLow;
    unsigned short SegmentSelector;
    unsigned char Reserved;
    unsigned char Flags;
    unsigned short BaseHigh;
} __attribute__((packed)) IDTEntry;
typedef struct {
    unsigned short Limit;
    unsigned int base;
}__attribute__((packed)) IDTDescriptor;
typedef enum {
    IDT_FLAG_GATE_TASK          = 0x5,
    IDT_FLAG_GATE_16BIT_INT     = 0x6,
    IDT_FLAG_GATE_16BIT_TRAP    = 0x7,
    IDT_FLAG_GATE_32BIT_INT     = 0xE,
    IDT_FLAG_GATE_32BIT_TRAP    = 0xF,

    IDT_FLAG_RING0              = (0 << 5),
    IDT_FLAG_RING1              = (1 << 5),
    IDT_FLAG_RING2              = (2 << 5),
    IDT_FLAG_RING3              = (3 << 5),

    IDT_FLAG_PRESENT            = 0x80,
} IDT_FLAGS;

__attribute((aligned(0x10))) IDTEntry IDT[256] = {0};
IDTDescriptor IDT_Desc;

void *interrupt_handlers[16] = { 0 };

void irq_remap(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
}

char *isrs[] = {
    "0x0001 Division By Zero",
	"0x0002 Debug",
	"0x0003 Non Maskable Interrupt",
	"0x0004 Breakpoint",
	"0x0005 Into Detected Overflow",
	"0x0006 Out of Bounds",
	"0x0007 Invalid Opcode",
	"0x0008 No FPU Coprocessor",
	"0x0009 Double Fault",
	"0x000A Coprocessor Segment Overrun",
	"0x000B Bad TSS",
	"0x000C Segment Not Present",
	"0x000D Stack Fault",
	"0x000E General Protection Fault",
	"0x000F Page Fault",
	"0x0000 (0x0010) \"How Did We Get Here?\"",
	"0x0011 Coprocessor Fault",
	"0x0012 Alignment Check",
	"0x0013 Machine Check",
	"0x0014 Reserved",
	"0x0015 Reserved",
	"0x0016 Reserved",
	"0x0017 Reserved",
	"0x0018 Reserved",
	"0x0019 Reserved",
	"0x001A Reserved",
	"0x001B Reserved",
	"0x001C Reserved",
	"0x001D Reserved",
	"0x001E Reserved",
	"0x001F Reserved",
	"0x0020 Reserved"
};


extern void _fhandler(registers *r)
{
    
    if (r->int_no < 32)
    {
        kprintf(isrs[r->int_no], 0x4f);
        kprintf(" Exception. System Halted!\n",  0x4f);
        if(r->int_no == 2){
            int system_port_a = 0x92;
            int system_port_b = 0x61;
            char sysctrla = inb(system_port_a);
            char sysctrlb = inb(system_port_b);
            if(sysctrla & 16){
                kprintf("Watchdog Timer\n", 0x4f);
            }
            if(sysctrlb & 0x40){
                kprintf("Channel Check, Bus error\n", 0x4f);
            }
            if(sysctrlb & 0x80){
                kprintf("Parity Check\n", 0x4f);
            }
        }
        for(;;);
    }
}
void irq_install_handler(int irq, void (*handler)(registers *r))
{
	interrupt_handlers[irq] = (void*)handler;
}
unsigned char currentInterrupt = -1;
extern void _irq_handler(registers *r)
{
	currentInterrupt = r -> int_no;
    if(currentInterrupt == 0x80){

    }
    else{
        void (*handler)(registers *r);

	
        handler = (void (*)(registers*))interrupt_handlers[r->int_no - 32];
        if (handler)
        {
            
            handler(r);
        }

        if (r->int_no >= 40)
        {
            outb(0xA0, 0x20);
        }

        outb(0x20, 0x20);
    }
	
}

void idt_set(unsigned char num, void *isr, unsigned char flags){
    IDT[num].BaseLow = (unsigned int)isr & 0xffff;
    IDT[num].SegmentSelector = 0x8;
    IDT[num].BaseHigh = (unsigned int)isr >> 16 & 0xffff;
    IDT[num].Flags = flags;
    IDT[num].Reserved = 0;
}

extern void isr_stub();
extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void softint();

extern void load_idt();


void idt_init(){
    IDT_Desc.base = (unsigned int)&IDT;
    IDT_Desc.Limit = ((unsigned short)sizeof(IDTDescriptor) * 256) - 1;
   idt_set(0, _isr0, 0x8e);
    idt_set(1, _isr1, 0x8e);
    idt_set(2, _isr2, 0x8e);
    idt_set(3, _isr3, 0x8e);
    idt_set(4, _isr4, 0x8e);
    idt_set(5, _isr5, 0x8e);
    idt_set(6, _isr6, 0x8e);
    idt_set(7, _isr7, 0x8e);
    idt_set(8, _isr8, 0x8e);
    idt_set(9, _isr9, 0x8e);
    idt_set(10, _isr10, 0x8e);
    idt_set(11, _isr11, 0x8e);
    idt_set(12, _isr12, 0x8e);
    idt_set(13, _isr13, 0x8e);
    idt_set(14, _isr14, 0x8e);
    idt_set(15, _isr15, 0x8e);
    idt_set(16, _isr16, 0x8e);
    idt_set(17, _isr17, 0x8e);
    idt_set(18, _isr18, 0x8e);
    idt_set(19, _isr19, 0x8e);
    idt_set(20, _isr20, 0x8e);
    idt_set(21, _isr21, 0x8e);
    idt_set(22, _isr22, 0x8e);
    idt_set(23, _isr23, 0x8e);
    idt_set(24, _isr24, 0x8e);
    idt_set(25, _isr25, 0x8e);
    idt_set(26, _isr26, 0x8e);
    idt_set(27, _isr27, 0x8e);
    idt_set(28, _isr28, 0x8e);
    idt_set(29, _isr29, 0x8e);
    idt_set(30, _isr30, 0x8e);
    idt_set(31, _isr31, 0x8e);
    
    irq_remap();
    idt_set(32, irq0, 0x8E);
	idt_set(33, irq1, 0x8E);
	idt_set(34, irq2, 0x8E);
	idt_set(35, irq3, 0x8E);
	idt_set(36, irq4, 0x8E);
	idt_set(37, irq5, 0x8E);
	idt_set(38, irq6, 0x8E);
	idt_set(39, irq7, 0x8E);
	idt_set(40, irq8, 0x8E);
	idt_set(41, irq9, 0x8E);
	idt_set(42, irq10, 0x8E);
	idt_set(43, irq11, 0x8E);
	idt_set(44, irq12, 0x8E);
	idt_set(45, irq13, 0x8E);
	idt_set(46, irq14, 0x8E);
	idt_set(47, irq15, 0x8E);
    idt_set(0x80, softint, 0b11101111);
    
    outb(0x21, 0xfd);
    outb(0xa1, 0xff);
    asm("sti");
    load_idt();
}