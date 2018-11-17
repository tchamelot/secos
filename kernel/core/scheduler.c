#include <debug.h>
#include <segmem.h>
#include <gpr.h>
#include <intr.h>
#include <scheduler.h>
#include <task.h>

extern task_t tasks[2];
extern volatile uint8_t state;
extern uint32_t* counter;
static task_t* current_task = tasks;

void scheduler_hdl(int_ctx_t* ctx)
{
	//debug("Enter scheduler, %x, %u\n", state, *counter);
	if((ctx->cs.raw & 0x3) != 0)
	{
		save_task(current_task, ctx);
		current_task = current_task->next_task;
		restore_task(current_task);
	}
}
