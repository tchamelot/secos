#ifndef _TASK_H_
#define _TASK_H_

#include <string.h>
#include "seg_init.h"

#define __user1__  __attribute__((section(".ucode"),aligned(16)))
#define __user2__  __attribute__((section(".ucode2"),aligned(16)))

#define TASK_INIT	0x01
#define TASK_RUNNING 	0x02

typedef struct task_t task_t;
struct task_t
{
	char*		name;
	uint32_t* 	code;
	uint32_t	ustack;
	uint32_t 	kstack;

	tss_t*		ts;
	seg_sel_t 	ts_sel;
	uint32_t 	cr3;	

	uint8_t		state;
	task_t*		next_task;
};

//void init_task(task_t* task, tss_t ts, uint16_t ts_sel, uint32_t* code, uint32_t kstack, uint32_t ustack);
void init_task(task_t* task);
void switch_task(task_t* task);
void prepare_task(task_t* task);

#endif
