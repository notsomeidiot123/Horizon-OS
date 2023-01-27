#include "memory.h"
#include "linear.h"
//allows space between kernel and EBDA
#define MEM_PAGE_TABLE_START 0x20000;
#define NULL (void *)0
#pragma once
void init_master_page_table();
struct 
{
    unsigned int maximum_memory;
    unsigned int page_table_size;
    unsigned char page_size;
    unsigned int *rfs_ptr;
} mmanager;

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
    unsigned char used : 1;
    unsigned char dirty : 1;
    unsigned char system : 1;
    unsigned char permissions : 3;
    unsigned char linked_next : 1;
    unsigned char reserved : 1;
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
    unsigned char *page_dir_table_start;
    struct{
        char bits : 1; //0 for 32 bits, 1 for 64
        char pae_en : 1;//must be 0 for long mode
        char paging : 1;//1 if paging is enabled else use linear
        char v8086  : 1;//not a memory mode, but helpful if running in vm8086 mode
        char reserved : 4;
    }memory_type;
} mem_descriptor;

int initram(unsigned int ent_count, mmap_ent *restrict mmap_ptr){
    // mmap_ent *mmap_arr = (mmap_ent *)mmap_ptr;
    unsigned int usable_ram = 0;
    
    mem_descriptor.page_dir_table_start = (unsigned char *)MEM_PAGE_TABLE_START;
    for(int i = 0; i < ent_count; i++){
        // kprintf("%d : %x | %x\n", mmap_arr[i].type, mmap_arr[i].base_low, mmap_arr[i].limit_low + mmap_arr[i].base_low);
        if(mmap_ptr[i].type == 1){
            // kprintf("%d : %x | %x\n", mmap_arr[i].type, mmap_arr[i].base_low, mmap_arr[i].limit_low);
            usable_ram += mmap_ptr[i].limit_low;
            mem_descriptor.page_dir[mem_descriptor.page_dir_index].base = mmap_ptr[i].base_low;
            mem_descriptor.page_dir[mem_descriptor.page_dir_index].length = mmap_ptr[i].limit_low;
            // table_start += (mmap_arr[i].limit_low/4096) + 1 * sizeof(page_table_ent);
            mem_descriptor.page_dir[mem_descriptor.page_dir_index].table_length = (mmap_ptr[i].limit_low/4096) + 1 * sizeof(page_table_ent);
            mem_descriptor.page_dir_index++;
            
            
            if(mmap_ptr[i].base_low < mem_descriptor.base_useable){
                mem_descriptor.base_useable = mmap_ptr[i].base_low;
            }
            if(mmap_ptr[i].limit_low > mem_descriptor.max_addr){
                mem_descriptor.max_addr = mmap_ptr[i].limit_low;
            }
        }
        
    }
    if(!(usable_ram == 0)){
        
        init_master_page_table();
    }
    mem_descriptor.total_usable = usable_ram;
    return usable_ram;
}

void init_master_page_table(){
    unsigned int total_allocated_to_table = 0;
    for(int i = 0; i < mem_descriptor.page_dir_index; i++){
        mem_descriptor.page_dir[i].page_table = (page_table_ent *)MEM_PAGE_TABLE_START + total_allocated_to_table;
        mem_descriptor.page_dir[i].table_length = (mem_descriptor.page_dir[i].length/4096) * sizeof(page_table_ent);
        total_allocated_to_table += (mem_descriptor.page_dir[i].length/4096) * sizeof(page_table_ent);
    }
}
//allocates pages pages of consecutive memory (flat or virtual). if there is not enough consecutive memory, return null
void *kmalloc_page(unsigned int pages, unsigned char permissions){
    if(mem_descriptor.memory_type.paging){
        //paging code 
        return NULL;
    }
    if(pages > 1){
        if(pages > mem_descriptor.total_usable){
            return NULL;
        }
        else{
            for(int i =0; i < mem_descriptor.page_dir_index; i++){
                page_dir_ent dirent = mem_descriptor.page_dir[i];
                if(pages > dirent.table_length - dirent.used)
                for(int j = 0; j < dirent.table_length; i++){
                    
                }
            }
        }
    }
    else{
        for(int i = 0; i < mem_descriptor.page_dir_index; i++){
            page_dir_ent dirent = mem_descriptor.page_dir[i];
            if(dirent.used < dirent.table_length){
                for(int j = 0; j < dirent.table_length; j++){
                    if(!dirent.page_table[j].used && pages == 1){
                        dirent.page_table[j].dirty = 1;
                        dirent.page_table[j].used = 1;
                        dirent.page_table[j].system = permissions & 0x8;
                        dirent.page_table[j].permissions = permissions & 0x7;
                        dirent.page_table[j].linked_next = 0;
                    }
                }
                dirent.used ++;
            }
        }
        // The above code just allocates a single page, 
    }
}
void kfree(void *restrict ptr){
    if(!(mem_descriptor.memory_type.paging)){
        for(int i = 0; i < mem_descriptor.page_dir_index; i++){
            if((char *)ptr < (char *)mem_descriptor.page_dir[i].length + mem_descriptor.page_dir[i].base && (char *)ptr >= (char *)mem_descriptor.page_dir[i].base){
                int i = 0;
                while(mem_descriptor.page_dir[i].page_table[i+ (int)ptr/4096].linked_next){
                    mem_descriptor.page_dir[i].page_table[((int)ptr/4096)+i].used = 0;
                    mem_descriptor.page_dir[i].page_table[((int)ptr/4096)+i].linked_next = 0;
                    mem_descriptor.page_dir[i].page_table[((int)ptr/4096)+i].permissions = 0;
                    i++;
                }
            }
        }
    }
}