#pragma once
#include "../drivers/input/ps2.h"

extern void horizonShell();
extern int horizonShellInit();
extern unsigned int horizonShellTextInput(unsigned char c); 

unsigned int horizonShellTextInput(unsigned char c){
    
    kprintf("%c", c);
}