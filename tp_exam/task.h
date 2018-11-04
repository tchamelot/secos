#ifndef _TASK_H_
#define _TASK_H_

#include <string.h>
#include "seg_init.h"

#define __user1__  __attribute__((section(".ucode"),aligned(16)))
#define __user2__  __attribute__((section(".ucode2"),aligned(16)))


typedef struct
{
	uint32_t* 	code;
	uint32_t	ustack;
	uint32_t 	kstack;
	tss_t*		ts;
	seg_sel_t 	ts_sel;	
	uint8_t		state;
} task_t;

//void init_task(task_t* task, tss_t ts, uint16_t ts_sel, uint32_t* code, uint32_t kstack, uint32_t ustack);
void init_task(task_t* task);
#endif
