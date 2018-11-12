#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include <segmem.h>

void init_syscall();
void syscall_irq();
void __regparm__(1) syscall_handler(int_ctx_t* ctx);

void take_mutex(uint32_t* mutex);
void release_mutex(uint32_t* mutex);
void print_counter(uint32_t* counter);

void take_mutex_hdl(uint32_t* mutex);
void release_mutex_hdl(uint32_t* mutex);
void print_counter_hdl(uint32_t* counter);
#endif
