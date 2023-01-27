#include "../memory/memory.h"
#include "../vga/graphics.h"
#pragma once

#define MASTER_PIC 0x20
#define MASTER_PIC_DATA 0x21
#define SLAVE_PIC 0xa0
#define SLAVE_PIC_DATA 0xa1
#define PIC_EOI 0x20

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



typedef struct
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} registers;

extern void _fhandler(registers *r)
{
    kprintf("FhandlerCalled\n");
    if (r->int_no < 32)
    {
        // kprintf(isrs[r->int_no], 0x4f);
        // kprintf(" Exception. System Halted!\n",  0x4f);
        const char *execptions[] = {
            "0x0001 Division By Zero\0",
	        "0x0002 Debug\0",
	        "0x0003 Non Maskable Interrupt\0",
	        "0x0004 Breakpoint\0",
	        "0x0005 Into Detected Overflow\0",
	        "0x0006 Out of Bounds\0",
	        "0x0007 Invalid Opcode\0",
	        "0x0008 No FPU Coprocessor\0",
	        "0x0009 Double Fault",
	        "0x000A Coprocessor Segment Overrun\0",
	        "0x000B Bad TSS\0",
	        "0x000C Segment Not Present\0",
	        "0x000D Stack Fault\0",
	        "0x000E General Protection Fault\0",
	        "0x000F Page Fault\0",
	        "0x0000 (0x0010) \"How Did We Get Here?\"\0",
	        "0x0011 Coprocessor Fault\0",
	        "0x0012 Alignment Check\0",
	        "0x0013 Machine Check\0",
	        "0x0014 Reserved\0",
	        "0x0015 Reserved\0",
	        "0x0016 Reserved\0",
	        "0x0017 Reserved\0",
	        "0x0018 Reserved\0",
	        "0x0019 Reserved\0",
	        "0x001A Reserved\0",
	        "0x001B Reserved\0",
	        "0x001C Reserved\0",
	        "0x001D Reserved\0",
	        "0x001E Reserved\0",
	        "0x001F Reserved\0",
	        "0x0020 Reserved\0"
        };
        // if(r->int_no == 2){
        //     int system_port_a = 0x92;
        //     int system_port_b = 0x61;
        //     char sysctrla = inb(system_port_a);
        //     char sysctrlb = inb(system_port_b);
        //     if(sysctrla & 16){
        //         kprintf("Watchdog Timer\n", 0x4f);
        //     }
        //     if(sysctrlb & 0x40){
        //         kprintf("Channel Check, Bus error\n", 0x4f);
        //     }
        //     if(sysctrlb & 0x80){
        //         kprintf("Parity Check\n", 0x4f);
        //     }
        // }
        kprintf("\nFatal Error: %s Exception\nYou might need a little bit of help </3. Here's some debug info:\n", execptions[r->int_no]);
        kprintf("EAX: %x EBX: %x ECX: %x EDX: %x\nEIP: %x CS: %x DS: %x\nEBP: %x ESP: %x SS: %x\n", r->eax, r->ebx, r->ecx, r->edx, r->eip, r->cs, r->ds, r->ebp, r->esp, r->ss);
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

void idt_set(unsigned char num, unsigned int isr, unsigned char flags){
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
    idt_set(0, (unsigned)_isr0, 0x8e);
    idt_set(1, (unsigned)_isr1, 0x8e);
    idt_set(2, (unsigned)_isr2, 0x8e);
    idt_set(3, (unsigned)_isr3, 0x8e);
    idt_set(4, (unsigned)_isr4, 0x8e);
    idt_set(5, (unsigned)_isr5, 0x8e);
    idt_set(6, (unsigned)_isr6, 0x8e);
    idt_set(7, (unsigned)_isr7, 0x8e);
    idt_set(8, (unsigned)_isr8, 0x8e);
    idt_set(9, (unsigned)_isr9, 0x8e);
    idt_set(10, (unsigned)_isr10, 0x8e);
    idt_set(11, (unsigned)_isr11, 0x8e);
    idt_set(12, (unsigned)_isr12, 0x8e);
    idt_set(13, (unsigned)_isr13, 0x8e);
    idt_set(14, (unsigned)_isr14, 0x8e);
    idt_set(15, (unsigned)_isr15, 0x8e);
    idt_set(16, (unsigned)_isr16, 0x8e);
    idt_set(17, (unsigned)_isr17, 0x8e);
    idt_set(18, (unsigned)_isr18, 0x8e);
    idt_set(19, (unsigned)_isr19, 0x8e);
    idt_set(20, (unsigned)_isr20, 0x8e);
    idt_set(21, (unsigned)_isr21, 0x8e);
    idt_set(22, (unsigned)_isr22, 0x8e);
    idt_set(23, (unsigned)_isr23, 0x8e);
    idt_set(24, (unsigned)_isr24, 0x8e);
    idt_set(25, (unsigned)_isr25, 0x8e);
    idt_set(26, (unsigned)_isr26, 0x8e);
    idt_set(27, (unsigned)_isr27, 0x8e);
    idt_set(28, (unsigned)_isr28, 0x8e);
    idt_set(29, (unsigned)_isr29, 0x8e);
    idt_set(30, (unsigned)_isr30, 0x8e);
    idt_set(31, (unsigned)_isr31, 0x8e);
    irq_remap();
    idt_set(32, (unsigned)irq0, 0x8E);
	idt_set(33, (unsigned)irq1, 0x8E);
	idt_set(34, (unsigned)irq2, 0x8E);
	idt_set(35, (unsigned)irq3, 0x8E);
	idt_set(36, (unsigned)irq4, 0x8E);
	idt_set(37, (unsigned)irq5, 0x8E);
	idt_set(38, (unsigned)irq6, 0x8E);
	idt_set(39, (unsigned)irq7, 0x8E);
	idt_set(40, (unsigned)irq8, 0x8E);
	idt_set(41, (unsigned)irq9, 0x8E);
	idt_set(42, (unsigned)irq10, 0x8E);
	idt_set(43, (unsigned)irq11, 0x8E);
	idt_set(44, (unsigned)irq12, 0x8E);
	idt_set(45, (unsigned)irq13, 0x8E);
	idt_set(46, (unsigned)irq14, 0x8E);
	idt_set(47, (unsigned)irq15, 0x8E);
    idt_set(0x80, (unsigned)softint, 0b11101111);
    
    outb(0x21, 0xfd);
    outb(0xa1, 0xff);
    asm("sti");
    load_idt();
}

enum PIC_MASKS_MASTER{
    MASK_PIT = 1,
    MASK_PS2KB = 2,
    MASK_CASCADE = 4,
    MASK_COM2 = 8,
    MASK_COM1 = 16,
    MASK_LPT2 = 32,
    MASK_FLOPPY = 64,
    MASK_LPT1 = 128,
    MASK_CMOS = 1,
    MASK_PERIF_0 = 2,
    MASK_PERIF_1 = 4,
    MASK_PERIF_2 = 8,
    MASK_PS2M = 16,
    MASK_FPU = 32,
    MASK_PATA_P = 64,
    MASK_PATA_S = 128
};

union{
    unsigned char byte;
    struct{
        unsigned char pit: 1;
        unsigned char ps2kb: 1;
        unsigned char cascade: 1;
        unsigned char com2: 1;
        unsigned char com1: 1;
        unsigned char lpt2: 1;
        unsigned char floppy: 1;
        unsigned char lpt1: 1;
    }data;
}pic_mask_master;
union{
    unsigned char byte;
    struct{
        unsigned char cmos: 1;
        unsigned char perif_0: 1;
        unsigned char perif_1: 1;
        unsigned char perif_2: 1;
        unsigned char ps2m: 1;
        unsigned char fpu: 1;
        unsigned char pata_p: 1;
        unsigned char pata_s: 1;
    }data;
}pic_mask_slave;
void pic_remask(unsigned char master_mask, char slave_mask){
    outb(0x21, !master_mask);
    outb(0xa1, !slave_mask);
}
void pic_disable(){
    outb(0x21, 0xff);
    outb(0xa1, 0xff);
}