#include <debug.h>
#include <segmem.h>
#include <gpr.h>
#include <intr.h>
#include "scheduler.h"
#include "seg_init.h"
#include "task.h"

extern task_t tasks[2];

void init_scheduler()
{
	idt_reg_t idtr;

	get_idtr(idtr);

	idtr.desc[32].offset_1 = (uint16_t)((uint32_t)scheduler_irq);
	idtr.desc[32].offset_2 = (uint16_t)(((uint32_t)scheduler_irq)>>16);
}

void scheduler_irq()
{
	// Je préfaire le jijutsu ^^
	asm volatile("leave");

	asm volatile("pushl $-1");
	asm volatile("push $32");

	asm volatile("push %eax");
	asm volatile("movb $0x20, %al");
	asm volatile("outb %al, $0x20");
	asm volatile("pop %eax");

	asm volatile("pusha");
	asm volatile("mov %esp, %eax");
	asm volatile("call scheduler_handler");
	
	asm volatile("popa");
	asm volatile("add $8, %esp");
	asm volatile("iret");
}

void __regparm__(1) scheduler_handler(int_ctx_t* ctx)
{
	static uint32_t current_task = 0;

	if((ctx->cs.raw & 0x3) != 0)
	{
		if((tasks[current_task].state & TASK_RUNNING) == TASK_RUNNING)
		{
			tasks[current_task].kstack = get_esp();
			tasks[current_task].state &= !TASK_RUNNING;
		}

		if((tasks[next_task].state & TASK_INIT) != TASK_INIT)
		{
			switch_task(&tasks[next_task]);
		}
		else
		{
			tasks[next_task].state |= TASK_RUNNING;
			tasks[next_task].ts->s0.esp = tasks[next_task].kstack;
			set_tr(tasks[next_task].ts_sel);
			set_esp(tasks[next_task]);
		}
	}
}
