#pragma once
#include "../devices.h"
#include "../../vga/graphics.h"

#define CONTROL ^ 8 | 0x206

enum ATA_PORTS{
    ATA_PRIMARY=0x1F0,
    ATA_SECONDARY=0x170,
    ATA_TERTIARY = 0x1E8,
    ATA_QUATERNARY = 0x168
};


void ata_identify(){
    
}