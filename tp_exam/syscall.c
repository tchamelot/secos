#include <debug.h>
#include <segmem.h>
#include <intr.h>
#include "syscall.h"

void (*syscall[])(uint32_t*) = {
	take_mutex_hdl,
	release_mutex_hdl,
	print_counter_hdl
};

void init_syscall()
{
	idt_reg_t idt;

	get_idtr(idt);

	idt.desc[0x80].dpl = 3;
	idt.desc[0x80].offset_1 = (uint16_t)((uint32_t)syscall_irq);
	idt.desc[0x80].offset_2 = (uint16_t)((uint32_t)syscall_irq>>16);
}

void syscall_irq()
{
	asm volatile(
		"leave			\n"
		"pusha			\n"
		"mov %esp, %eax		\n"
		"call syscall_handler	\n"
		"popa			\n"
		"iret			\n"
	);
}

void __regparm__(1) syscall_handler(int_ctx_t* ctx)
{
	uint32_t nb_sys = ctx->gpr.eax.raw;
	if(nb_sys <= 0x2)
	{
		debug("[Syscall] nb : %x, param : %x\n", nb_sys, *(uint32_t*)ctx->gpr.ebx.raw);
		syscall[nb_sys]((uint32_t*)ctx->gpr.ebx.raw);
	}
}

void print_counter(uint32_t* counter)
{
	asm volatile("int $0x80"::"a"(0x2),"b"(counter));
}

void print_counter_hdl(uint32_t* counter)
{
	debug("Counter = %d\n", *counter);
}

void take_mutex(uint32_t* mutex)
{	
	asm volatile("int $0x80"::"a"(0x0),"b"(mutex));
}

void take_mutex_hdl(uint32_t* mutex)
{
	if((*mutex) == 0)
		(*mutex)++;

}

void release_mutex(uint32_t* mutex)
{
	asm volatile("int $0x80"::"a"(0x1),"b"(mutex));
}

void release_mutex_hdl(uint32_t* mutex)
{
	if((*mutex) > 0)
		(*mutex) = 0;
}
