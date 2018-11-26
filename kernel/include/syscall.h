#ifndef _SYSCALL_H_
#define _SYSCALL_H_

void syscall_hdl();

void sys_counter(uint32_t* counter);
void sys_counter_hdl(uint32_t* counter);

uint32_t check_arg_ptr(uint32_t* arg);
#endif
