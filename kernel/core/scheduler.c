#include <intr.h>
#include <scheduler.h>
#include <task.h>

extern task_t tasks[2];
extern volatile uint8_t state;
extern uint32_t* counter;
static task_t* current_task = tasks;

/**
 * \fn			void scheduler_hdl(int_ctx_t* ctx)
 * \brief		High level scheduler handler mapped on irq 32.
 *
 * \param ctx	Interruption context from irq32
 */
void scheduler_hdl(int_ctx_t* ctx)
{
	if((ctx->cs.raw & 0x3) != 0)
	{
		save_task(current_task, ctx);
		current_task = current_task->next_task;
		restore_task(current_task);
	}
}
