#include"../vga/graphics.h"
#pragma once


typedef struct{
    unsigned int base_low;
    unsigned int base_high;
    unsigned int limit_low;
    unsigned int limit_high;
    unsigned int type;
    unsigned int ExtendedAttrib;
} mmap_ent;

typedef struct
{
    unsigned char free : 1;
    unsigned char dirty : 1;
    unsigned char system : 1;
    unsigned char permissions : 3;
    unsigned char reserved : 2;
}page_table_ent;


typedef struct
{
    unsigned int base;
    unsigned int length;
    page_table_ent* page_table;
    unsigned int table_length;
    unsigned int used;
    //interpolate ptr by index * 4096 + base
}page_dir_ent;


struct{
    unsigned int max_addr;
    unsigned int total_usable;
    unsigned int base_useable;
    page_dir_ent page_dir[32];
    unsigned char page_dir_index;
    unsigned char page_dir_table_start;
} mem_descriptor;


int initram(unsigned int ent_count, mmap_ent *mmap_ptr){
    mmap_ent *mmap_arr = (mmap_ent *)mmap_ptr;
    unsigned int usable_ram = 0;
    unsigned int table_start = 0x20000;
    mem_descriptor.page_dir_table_start = table_start;
    for(int i = 0; i < ent_count; i++){
        // kprintf("%d : %x | %x\n", mmap_arr[i].type, mmap_arr[i].base_low, mmap_arr[i].limit_low + mmap_arr[i].base_low);
        if(mmap_arr[i].type == 1){
            // kprintf("%d : %x | %x\n", mmap_arr[i].type, mmap_arr[i].base_low, mmap_arr[i].limit_low);
            usable_ram += mmap_arr[i].limit_low;
            mem_descriptor.page_dir[mem_descriptor.page_dir_index].base = mmap_arr[i].base_low;
            mem_descriptor.page_dir[mem_descriptor.page_dir_index].length = mmap_arr[i].limit_low;
            mem_descriptor.page_dir[mem_descriptor.page_dir_index].page_table = table_start;
            table_start += (mmap_arr[i].limit_low/4096) + 1 * sizeof(page_table_ent);
            mem_descriptor.page_dir[mem_descriptor.page_dir_index].table_length = (mmap_arr[i].limit_low/4096) + 1 * sizeof(page_table_ent);
            if(mmap_arr[i].base_low < mem_descriptor.base_useable){
                mem_descriptor.base_useable = mmap_arr[i].base_low;
            }
            if(mmap_arr[i].limit_low > mem_descriptor.max_addr){
                mem_descriptor.max_addr = mmap_arr[i].limit_low;
            }
        }
        
    }
    mem_descriptor.total_usable = usable_ram;
    return usable_ram;
}