# Errors

In the case of a Kernel Error during runtime of the OS, an error message will be displayed. There are different types of errors and error messages depending on the circumstances. Since v0.1.0, error messages are currently displayed and written the same way as any other text, into STDOUT, but there are plans to add a special error display caller to accomodate for GUI and non-defualt terminal shells.

## Faults/Kernel Panic

|Code|Meaning|
|----|-------|
0x0001 | Division by 0
0x0002 | Debug
0x0003 | Non-maskable Interrupt
0x0004 | Breakpoint
0x0005 | Overflow
0x0006 | Out of Bounds
0x0007 | Invalid Opcode
0x0008 | No FPU Coprocessor
0x0009 | Double Fault
0x000A | Coproc Segment Overrun
0x000B | Bad TSS
0x000C | Segment not Present
0x000D | Stack Fault
0x000E | GP Fault
0x000F | Page Fault
0x0010 | Unreachable
0x0011 | Coprocessor Fault
0x0012 | Alignment Check
0x0013 | Machine Check
0x0014 | Start of Reserved Faults
0x0020 | End of Reserved Faults

### What to do if any of these happen?

In the event that one of these faults occur, assuming there is no triple fault, the best course of action is to reboot, and disable all Kernel-Space programs, AKA Safe-mode, which will be able to be accessed by pressing (spamming) the F4 Key on boot. pressing ctrl+shift+esc while in TTY/Default Shell will drop into safe mode automatically[^1].

In the event that you cannot boot or drop into safe mode, or even after disabling all Kernel-space programs, the following options are reccomended in this order. Go down the list until you find one that works


- Disable all installed Kernel-space programs. This includes
    - Shells
    - File Managers
    - Memory Managers
    - Device Drivers
- Reinstall HorizonBootMGR[^4]
- Install/Reinstall KiminoMBR[^4]
- Factory reset your system. Depending whether or not you can access safe mode[^2].
- Boot to another OS (Preferably *nix), and follow the above steps
- Reinstall
- Upgrade your system, or if on the latest version, Downgrade
- Look through the code, find the bug, and submit a PR[^3]



## ACPI Errors

|Code | Meaning|
|-|-|-|
0x1 | No RSDP/ACPI not supported
0x2 | Unknown ACPI Table Found
0x3 | Error Parsing Table
0x4 | No Parser
0x5 | NULL Table Pointer

These errors generally occur during the final boot process, as seen in [kmain.c](/src/kernel/kmain.c)

In the event that ACPI is not supported on your system, You will recieve an ACPI error message, but please disregard it. HorizonOS has[^5] a backup process for detecting hardware as safe as possible. Some hardware will not be detected, as to avoid system crashes, but as much hardware as possible will be detected.



[^1]: Has not been added, but will be in the future
[^2]: May have to be done manually.
[^3]: This is a joke, but I'd love any sort of contribution if you're willing to help :)
[^4]: HorizonOS has been made with these bootloaders and the way they work in mind. Some bootloaders may give the same end result, but HorizonOS is tested with the default provided boot loaders. If there is a bug with these, please submit an issue [Here for KiminoMBR](https://github.com/notsomeidiot123/KiminoBootLoader/issues/new) and [Here for HorizonOS/HorizonBootMGR](https://github.com/notsomeidiot123/Horizon-OS/issues/new)
[^5]: Will have (2023-01-30)