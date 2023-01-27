#include "../idt.h"
#include "../../vga/graphics.h"
#pragma once
#include "../devices.h"

#define KBD_CONTROLLER_CMD 0x64
#define KBD_CONTROLLER_DATA 0x60
int init_ps_2_kbd(unsigned char scancode_set, void (*shell_input_handler)(unsigned char character)){
    
    
    
    
    return 1;
}

unsigned char keys[104] = {0};
unsigned char show_output = 0;

enum UNKNOWNS{
    KEY_NULL,
    KEY_ESCAPE,
    KEY_L_CONTROL,
    KEY_L_SHIFT,
    KEY_R_SHIFT,
    KEY_L_ALT,
    KEY_CAPS,
    KEY_F1,
    KEY_BS,
    KEY_TAB,
    KEY_LF,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_NUMLOCK,
    KEY_SCROLLLOCK,
    KEY_F11,
    KEY_F12,
};

unsigned char keymap_type = 0;
unsigned char keymap0[] = {
    0, KEY_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b', '\t', 'q','w','e', 'r','t','y','u','i','o','p',
    '[',']','\n', KEY_L_CONTROL,'a','s','d','f','g','h','j','k','l',';','\'',
    '`',KEY_L_SHIFT,'\\','z','x','c','v','b','n','m',',','.','/',
    KEY_R_SHIFT, '*', KEY_L_ALT, ' ', KEY_CAPS, KEY_F1, KEY_F2, KEY_F3,
    KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUMLOCK,
    KEY_SCROLLLOCK, '7','8','9','-','4','5','6','+','1','2','3','0','.',
    0, 0, 0, KEY_F11, KEY_F12
};


void (*shell_input_handler)(unsigned char character);

char get_key(unsigned char key){
    return keys[key];
}


unsigned char shift_key(char c){
    if(c > 0x60 && c < 0x7b){
        return c - 0x20;
    }
    switch (c)
    {
        // NO I DID NOT WRITE THIS... I COPY AND PASTED
        // I KNOW IM BAD AT PROGRAMMING
        case '1':
            return '!';
            break;
        case '2':
            return '@';
            break;
        case '3':
            return '#';
            break;
        case '4':
            return '$';
            break;
        case '5':
            return '%';
            break;
        case '6':
            return '^';
            break;
        case '7':
            return '&';
            break;
        case '8':
            return '*';
            break;
        case '9':
            return '(';
            break;
        case '0':
            return ')';
            break;
        case '-':
            return '_';
            break;
        case '=':
            return '+';
            break;
        case '[':
            return '{';
            break;
        case ']':
            return '}';
            break;
        case '\\':
            return '|';
            break;
        case ';':
            return ':';
            break;
        case '\'':
            return '\"';
            break;
        case ',':
            return '<';
            break;
        case '.':
            return '>';
            break;
        case '/':
            return '?';
            break;
        case '`':
            return '~';
            break;
    }
    return c;
}

void ps2kbd_irq(registers *r){
    static char mod;
    static char shift_pressed;
    static char caps_en;
    static char control_pressed;
    if(keymap_type == 0){
        unsigned int data = inb(KBD_CONTROLLER_DATA);
        if(data == 0xe0){
            mod = !mod;
        }
        else if(!(data & 0x80)){
            char keymap_data = keymap0[data];
            char out = 0;
            if(keymap_data == KEY_R_SHIFT || keymap_data == KEY_L_SHIFT){
                shift_pressed = 1;
                return;
            }
            else if(keymap_data == KEY_L_CONTROL){
                control_pressed = 1;
                return;
            }
            if(shell_input_handler){
                if(shift_pressed || caps_en){
                    shell_input_handler(shift_key(keymap_data));
                }else{
                    shell_input_handler(keymap_data);
                }
            }else if(vga_desc.textmode){
                if(control_pressed){
                    if(keymap_data == '\b'){
                        keymap_data = '\r';
                    }
                }
                kprintf("%c", shift_pressed ? shift_key(keymap_data) : keymap_data);
            }
        }
        else if(data & 0x80){
            int index = data ^0x80;
            keys[keymap0[index]] = 0;
            if(keymap0[index] == (KEY_L_SHIFT)|| keymap0[index] == (KEY_R_SHIFT)){
                shift_pressed = 0;
            }
            else if(keymap0[index] == KEY_L_CONTROL){
                control_pressed = 0;
            }
        }
    }
    return;
}
