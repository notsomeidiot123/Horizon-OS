

extern "C" void horizonShell(){
    unsigned short *bf = (unsigned short *)0xb8000;
    *bf = 0x0f41;
    //IT WORKS >:) I JUST LINKED C++ TO C VIA A C HEADER CAUSE IM STUPID
}