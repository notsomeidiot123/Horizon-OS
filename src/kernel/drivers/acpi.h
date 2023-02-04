#pragma once
#include "../memory/memory.h"
#include "../memory/mmanager.h"
#include "../vga/graphics.h"
// #include "devices.h"

// enum ACPI_TABLE_TYPE{
//     ACPI_NULL,
//     ACPI_MADT,
//     ACPI_DSDT,
//     ACPI_FADT,
//     ACPI_SRAT,
//     ACPI_SSDT,
//     ACPI_XSDT,
//     ACPI_RSDT,
//     ACPI_HPET,
//     ACPI_UNKN
// };
unsigned int acpi_version;
struct RSDPointer{
    char Signature[8];
    unsigned char Checksum;
    char OEMID[6];
    unsigned char Revision;
    unsigned int RsdtAddress;
}__attribute__((packed)) *RSDP;
struct ACPISDTHeader {
    char Sig[4];
    unsigned int len;
    unsigned char revision;
    unsigned char checksum;
    char OEMID[6];
    char OERMTableID[8];
    unsigned int OEMRev;
    unsigned int creatorID;
    unsigned int createrRev;
}__attribute__((packed));
struct GenericAddressStructure
{
  unsigned char AddressSpace;
  unsigned char BitWidth;
  unsigned char BitOffset;
  unsigned char AccessSize;
  unsigned long Address;
};
struct FADT
{
    struct   ACPISDTHeader h;
    unsigned int FirmwareCtrl;
    unsigned int Dsdt;
 
    // field used in ACPI 1.0; no longer in use, for compatibility only
    unsigned char  Reserved;
 
    unsigned char  PreferredPowerManagementProfile;
    unsigned short SCI_Interrupt;
    unsigned int SMI_CommandPort;
    unsigned char  AcpiEnable;
    unsigned char  AcpiDisable;
    unsigned char  S4BIOS_REQ;
    unsigned char  PSTATE_Control;
    unsigned int PM1aEventBlock;
    unsigned int PM1bEventBlock;
    unsigned int PM1aControlBlock;
    unsigned int PM1bControlBlock;
    unsigned int PM2ControlBlock;
    unsigned int PMTimerBlock;
    unsigned int GPE0Block;
    unsigned int GPE1Block;
    unsigned char  PM1EventLength;
    unsigned char  PM1ControlLength;
    unsigned char  PM2ControlLength;
    unsigned char  PMTimerLength;
    unsigned char  GPE0Length;
    unsigned char  GPE1Length;
    unsigned char  GPE1Base;
    unsigned char  CStateControl;
    unsigned short WorstC2Latency;
    unsigned short WorstC3Latency;
    unsigned short FlushSize;
    unsigned short FlushStride;
    unsigned char  DutyOffset;
    unsigned char  DutyWidth;
    unsigned char  DayAlarm;
    unsigned char  MonthAlarm;
    unsigned char  Century;
 
    // reserved in ACPI 1.0; used since ACPI 2.0+
    unsigned short BootArchitectureFlags;
 
    unsigned char  Reserved2;
    unsigned int Flags;
 
    // 12 byte structure; see below for details
    struct GenericAddressStructure ResetReg;
 
    unsigned char  ResetValue;
    unsigned char  Reserved3[3];
 
    // 64bit pointers - Available on ACPI 2.0+
    unsigned long                X_FirmwareControl;
    unsigned long                X_Dsdt;
 
    struct GenericAddressStructure X_PM1aEventBlock;
    struct GenericAddressStructure X_PM1bEventBlock;
    struct GenericAddressStructure X_PM1aControlBlock;
    struct GenericAddressStructure X_PM1bControlBlock;
    struct GenericAddressStructure X_PM2ControlBlock;
    struct GenericAddressStructure X_PMTimerBlock;
    struct GenericAddressStructure X_GPE0Block;
    struct GenericAddressStructure X_GPE1Block;
}__attribute__((packed)) *FADT_PRESERVED;
enum ACPI_TABLE_TYPE{
    ACPI_NULL,
    ACPI_MADT,
    ACPI_DSDT,
    ACPI_FADT,
    ACPI_SRAT,
    ACPI_SSDT,
    ACPI_XSDT,
    ACPI_RSDT,
    ACPI_HPET,
    ACPI_UNKN
};
int parseDSDT(struct ACPISDTHeader *header){
    kprintf("|-Parsing DSDT: ");
    int type = check_table_type(header);
    if(type != ACPI_DSDT){
        kprintf("\r|-DSDT: ACPI ERROR 0x3\n");
        return -1;
    }
    
    unsigned int length = header->len;
    // kprintf("%x ", length);
    return 0;
}