#include "../memory/memory.h"
#include <stdarg.h>
#pragma once

#define LOG(a, s) kprintf("[%s]: %s", #a, s);

struct{
    unsigned char textmode;
    unsigned char mode;
    unsigned int res_x;
    unsigned int res_y;
    unsigned int depth;

}vga_desc;

unsigned short *d_buffer;
unsigned int xpos = 0;
unsigned int ypos = 0;

union
{
    unsigned char colorData;
    struct {
        unsigned char bg: 4;
        unsigned char fg: 4;
    } theme;
} theme;

int debug_fb = 0;

#define TM_BLACK 0
#define TM_BLUE 1
#define TM_GREEN 2
#define TM_CYAN 3
#define TM_RED 4
#define TM_PURPLE 5
#define TM_BROWN 6
#define TM_LGRAY 7
#define LIGHT 8 |
#define TM_WHITE 0xf
#define TM_YELLOW 0xe


#define TM_X_RES_MAX 80
#define TM_Y_RES_MAX 25

void t_en_cursor(unsigned char start, unsigned char end){
    outb(0x3d4, 0xa);
    outb(0x3d5, (inb(0x3d5) & 0xc0) | start);
    outb(0x3d4, 0xb);
    outb(0x3d5, (inb(0x3d5) & 0xe0) | end);
}
void t_mov_cursor(int x, int y){
    unsigned short pos = y * TM_X_RES_MAX + x;
    outb(0x3d4, 0xf);
    outb(0x3d5, (unsigned char) (pos & 0xff));
    outb(0x3f4, 0xe);
    outb(0x3d5, (unsigned char )((pos >> 8) & 0xff));
}



void kputc(char j){
    if(vga_desc.textmode){
        if(j == '\n'){
            ypos++;
            xpos = 0;
            if(ypos == TM_Y_RES_MAX){
                ypos--;
                memcpy((char *)0xb8000 + TM_X_RES_MAX * 2, (char *)0xb8000, (TM_X_RES_MAX * TM_Y_RES_MAX * 2) - TM_X_RES_MAX * 2 );
                memset((char *)0xb8000 + ((TM_Y_RES_MAX-1) * TM_X_RES_MAX * 2), TM_X_RES_MAX * 2, 0);
            }
            return;
        }
        else if(j == '\r'){
            xpos = 0;
            return;
        }
        else if(j == '\b'){
            short * c =(short *)(0xb8000);
            c += (ypos * TM_X_RES_MAX) + --xpos;
            *c = theme.colorData << 8| ' ';
            t_mov_cursor(xpos, ypos);
            if(xpos < 0){
                xpos = TM_X_RES_MAX;
                ypos--;
            }
            return;
        }else if(j == '\t'){
            j = ' ';
        }
        short * c =(short *)(0xb8000);
        c += (ypos * TM_X_RES_MAX) + xpos++;
        *c = theme.colorData << 8| j;
        if(ypos == TM_Y_RES_MAX){
            ypos--;
            memcpy((char *)0xb8000 + TM_X_RES_MAX * 2, (char *)0xb8000, (TM_X_RES_MAX * TM_Y_RES_MAX * 2) - TM_X_RES_MAX * 2 );
            memset((char *)0xb8000 + ((TM_Y_RES_MAX-1) * TM_X_RES_MAX * 2), TM_X_RES_MAX * 2, 0);        }
        t_mov_cursor(xpos, ypos);
    }
    else{
        //terminal_output(c)
    }
}

void kputs(char *str){
    if(vga_desc.textmode){
        while(*str){
            kputc(*str++);
        }
    }
    else{
        //memcpy to stdout & call display
    }
    
}

char *convert(unsigned int num, int b){
    char base[] = "0123456789ABCDEF";

    char buffer[64] = {0};
    memset(buffer, 64, 0);
    char *ptr = &buffer[63];
    *ptr = 0;

    do
    {
        *--ptr = base[num%b];
        num /= b;
    }while(num);
    return(ptr);

}


void kclear_text(){
    memset((char *)0xb8000, TM_X_RES_MAX * TM_Y_RES_MAX * 2, 0);
    xpos = 0;
    ypos = 0;
}
//Kernel formatted print
void kprintf(char *str, ...){
    char *format = str;
    char *s;
    unsigned int i;
    va_list args;
    va_start(args, str);
    while(*format){
        if(*format != '%'){
            kputc(*format);
            format++;
            continue;
        }
        if(!format) return;
        format++;
        switch(*format){
            case 'c':
                i = va_arg(args, int);
                kputc(i);
                break;
            case 'd':
                i = va_arg(args, int);
                if( i < 0){
                    i *= -1;
                    kputc('-');
                }
                kputs(convert(i, 10));
                break;
            case 'x':
                i = va_arg(args, int);
                if( i < 0){
                    i *= -1;
                    // kputc('-');
                }
                kputs(convert(i, 16));
                break;
            case 'b':
                i = va_arg(args, int);
                // if( i < 0){
                //     i *= -1;
                //     // kputc('-');
                // }
                kputs(convert(i, 2));
                break;
            case 's':
                s = va_arg(args, char *);
                kprintf(s);
                break;
        }
        format++;
    }
    va_end(args);
}
void setTheme(unsigned char fg, unsigned char bg){
    theme.theme.bg = bg;
    theme.theme.fg = fg;
    return;
};

