#include "../devices.h"
#pragma once
#include "../../vga/graphics.h"
#include <stdint.h>

//essentially private constants that should not be used outside of this file lmao
//i can't believe im making essentially my own language lmao
#define CONTROL_BASE | 0x206
#define CONTROL(a) (~0x8 & a) CONTROL_BASE 
#define DATA_REG + 0
#define ERROR_REG + 1
#define FEATURES_REG + 1
#define SECTOR_COUNT + 2
#define LBA_LOW + 3
#define LBA_MID + 4
#define LBA_HIH + 5
#define DRIVE_SEL + 6
#define STATUS_REG + 7
#define COMMAND_REG + 7
#define ALT_STAT + 0
#define DEVICE_CONTROL + 0
#define DRIVE_ADDRESS + 1

#define STAT_ERR & 1
#define STAT_IDX & 2
#define STAT_CORR & 4
#define STAT_DRQ & 8
#define STAT_SRV & 0x10
#define STAT_DF & 0x20
#define STAT_RDY & 0x40
#define STAT_BSY & 0x80
// #define PRIMARY 0x1F0
// #define SECONDARY 0x1E6
#define ALT ^ 0x80
#define BUS_ONE 0x1F0
#define BUS_TWO 0x1E8




void ata_identify_all(){
    ata_identify(0xa0, BUS_ONE);
    ata_identify(0xb0, BUS_ONE);
    // ata_identify(0xa0, BUS_ONE ALT);
    // ata_identify(0xb0, BUS_ONE ALT);
}

int ata_identify(int d, int bus){
    unsigned short data[256] = {0};
    outb(bus DRIVE_SEL , d);
    for(int i = 0; i < 15; i++){
        inb(bus STATUS_REG);
    }
    // while(inb(BUS_ONE) STAT_RDY == 0 || )
    outb(bus SECTOR_COUNT, 0);
    outb(bus LBA_LOW, 0);
    outb(bus LBA_MID, 0);
    outb(bus LBA_HIH, 0);
    outb(bus COMMAND_REG, 0xEC);
    int stat = inb(bus STATUS_REG);
    if(!stat){
        return -1;
    }
    else if(stat & 1){
        //if status returns error
        //after this, append code for
        return -1;
    }
    if(inb(bus LBA_HIH) || inb(bus LBA_LOW)){
        kprintf("DETECTED UNSUPPORTED DRIVE TYPE\n");
        return -1;
    }
    if(stat & 0x80){
        while(inb(bus STATUS_REG) & 0x80);
    }
    // kprintf("This is where i read my data :/\n");
    for(int i = 0; i < 256; i++){
        while(!(inb(bus STATUS_REG) STAT_DRQ));
        data[i] = inw(bus DATA_REG);
    }
    kprintf("ATA: %x", data[0]);
    kprintf("LOW LBA Size: %x\nHiGH LBA Size: %x\n", data[60] ,data[61]);
    
}

#undef STAT_ERR
#undef STAT_IDX
#undef STAT_CORR
#undef STAT_DRQ
#undef STAT_SRV
#undef STAT_DF 
#undef STAT_RDY
#undef STAT_BSY
#undef BUS_ONE
#undef BUS_TWO
#undef PRIMARY
#undef SECONDARY
#undef CONTROL
#undef DATA_REG
#undef ERROR_REG
#undef FEATURES_REG
#undef SECTOR_COUNT
#undef LBA_LOW
#undef LBA_MID
#undef LBA_HIH
#undef DRIVE_HEAD
#undef STATUS_REG
#undef COMMAND_REG
#undef ALT_STAT
#undef DEVICE_CONTROL
#undef DRIVE_ADDRESS