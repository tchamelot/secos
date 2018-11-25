#ifndef _TASK_H_
#define _TASK_H_

#include <string.h>
#include <intr.h>

#define __counter__  __attribute__((section(".countercode"),aligned(16)))
#define __printer__  __attribute__((section(".printercode"),aligned(16)))

#define TASK_INIT	0x01
#define TASK_RUNNING 	0x02

typedef struct task_t task_t;
struct task_t
{
	int_ctx_t 	ctx;
	uint32_t 	kstack;
	
	uint32_t 	cr3;	
	
	uint8_t		state;
	task_t*		next_task;
};

void init_task(task_t* task, uint32_t code, 
	uint32_t kstack, uint32_t ustack, task_t* next_task);
void save_task(task_t* task, int_ctx_t* ctx);
void restore_task(task_t* task);
void __regparm__(1) switch_task(task_t* task);

#endif
