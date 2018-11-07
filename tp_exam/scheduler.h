#ifndef _SHCEDULER_H_
#define _SHCEDULER_H_

#include <segmem.h>
#include <intr.h>

#define next_task (current_task + 1) % sizeof(tasks)

void init_scheduler();
void scheduler_irq();
void __regparm__(1) scheduler_handler(int_ctx_t* ctx);

#endif
