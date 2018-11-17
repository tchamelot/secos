#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include <segmem.h>

void syscall_hdl();

void print_counter(volatile uint32_t* counter);
void print_counter_hdl(volatile uint32_t* counter);
#endif
