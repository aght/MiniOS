#ifndef KERNEL_H
#define KERNEL_H

//From the 256MB Available,
//256MB are for the kernel, and 256MB for Apps (Half for App, half for User stack)
#define SYS_SYSTEM_MEM_AVAILABLE    0x8000000   //128MB
#define SYS_APP_MAX_SIZE            0x4000000   //64MB
#define SYS_USER_STACK_MAX_SIZE     0x4000000   //64MB
#define SYS_LOADER_BUFFER_MAX_SIZE  0x2000000   //32MB

#endif
