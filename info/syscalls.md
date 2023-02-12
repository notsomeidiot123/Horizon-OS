# Syscalls

This is the list of userspace syscalls. Syscalls can be executed by using the int 0x80 instruction. arguments are passed via registers.

|Syscall|eax|ebx|ecx|edx|esi|edi|
|-------|---|---|---|---|---|---|
|sys_exit()|0|err_code||||
|sys_out|1|char *str|int len|
|sys_in|2|&char|
|sys_write|3|char *str|int len|int fd|
|sys_read|4|&char|int fd|