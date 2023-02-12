#pragma once
void memcpy(char *src, char *dest, unsigned int size){
    for(unsigned int i = 0; i < size; i++){
        dest[i] = src[i];
    }
}
void memset(char *dest, unsigned int size, unsigned char value){
    for(unsigned int i = 0; i < size; i++){
        *(dest++) = value;
    }
}
void outb(unsigned short port, unsigned char byte){
    asm volatile ("outb %0, %1" ::  "a"(byte), "Nd"(port) );
}
unsigned char inb(unsigned short port){
    unsigned char byte = 0;
    asm volatile ("inb %1, %0" :  "=a"(byte) : "Nd"(port) );
    return byte;
}
int strlen(char *str){
    int i = 0;
    while(*(str + i++));
    return i;
}
char strcmp(char *s, char *c){
    if(strlen(s) != strlen(c)){
        return 0;
    }
    while(*s != 0){
        if(*(s++) != *(c++)){
            return 0;
        }else{
            if(*(s-1) == *(c-1)){
                continue;
            }
        }
    }
    return 1;
}
/**
 * tmp = RSDP;
            struct ACPISDTHeader *header = (struct ACPISDTHeader *)RSDP->RsdtAddress;
    
*/