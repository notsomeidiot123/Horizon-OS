#pragma once
#include "../vga/graphics.h"
#include "../memory/mmanager.h"
char *stdin = 0;
char *stdin_base = 0;

void horizonShellTextInput(char c){
    *(stdin++) = c;
    kprintf("%c", c);
    if(c == '\n'){
        //shell_findpkg
        // kprintf("Enter\n");
        *--stdin = 0;
        
        //This works... for now... i'll have to add more support for stuff like backspace, \r, etc
        //plans: \r works the same as \n, except it does not do the interpreting
        //\b...\b, \t is just a space, and f1-f12 will be added later
        
        
        // if(strcmp(stdin_base, "clear")){
        //     kprintf("cleared");
        // }
        // for(int i = 0; i < stdin+1 - stdin_base; i++){
        //     kprintf("%c", stdin[-i]);
        // }
        // kprintf("%s | %d", stdin_base, strlen(stdin_base));
        stdin = stdin_base;
        return;
    }
}

void horizonShell(){
    
    stdin = (char *)kmalloc_page(1, 0xff);
    stdin_base = stdin;
    kprintf("Starting Shell...");
    enable_ps2kbd();
    for(;;);
}
//https://www.youtube.com/watch?v=_rY-Gam-IY4