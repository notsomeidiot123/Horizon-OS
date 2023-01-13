# Introduction

HAT32 is a modified FAT Filesystem, made mostly because I'm lazy.
While HAT32 is not directly compatible with FAT, it shares the same File allocation table.

## Differences

While there are not many differences in HAT vs FAT, HAT32 is designed to move away from the list of disk geometry and towards a more LBA-Oriented design. It is very similar to exFAT more than any other type

## Boot sector layout


|Offset|Name  |Length|Description|
|------|------|------|-----------|
|0 |3  |b_jmp|Jump to code (usually jmp short 0x23 nop)
|3 |8  |b_oem|OEM Identifier. Defined as " HAT32 " (Spaces included)
|11|2  |b_pbs|Physical block size (in bytes)
|13|1  |b_spc|Sectors per cluster
|14|4  |b_sft|Size of FAT (int sectors)
|18|4  |b_fta|LBA address of the FAT|
|22|12 |b_vlb|Volume Label
|34|1  |b_flg|[Flags](#flags)
|35|1  |b_res|Reserved
|36|1  |b_sig|HAT Signature (0x7f)
|37|4  |b_len|Partiton Length|
|41|471|b_cde|Boot Code

### Flags

Im working on defining useful flags for this File System, but with every revision or version, I will update this list to include the new flags and their meanings

<b>HAT32 v1</b>
|Bit|Hex|Name|Description|
|-|----|------|----------|
|0|0x01|f_rmve|Drive is removeable
|1|0x02|f_res0|Unused
|2|0x04|f_res1|Reserved
|3|0x08|f_root|Root partition
|4|0x10|f_ver0|OR with f_ver1 to get the HAT version (low bit)
|5|0x20|f_ver1|OR with f_ver0 to get the HAT version (high bit)
|6|0x40|f_res2|Reserved
|7|0x80|f_vald|Must be set to be valid

