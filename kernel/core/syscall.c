#include <debug.h>
#include <segmem.h>
#include <intr.h>
#include <syscall.h>

void (*syscall[])(volatile uint32_t*) = {
	print_counter_hdl
};

void syscall_hdl(int_ctx_t* ctx)
{
	uint32_t nb_sys = ctx->gpr.eax.raw;
	if(nb_sys < sizeof(syscall))
	{
		syscall[nb_sys]((volatile uint32_t*)ctx->gpr.ebx.raw);
	}
}

void print_counter(volatile uint32_t* counter)
{
	asm volatile("int $0x80"::"a"(0x0),"b"(counter));
}

void print_counter_hdl(volatile uint32_t* counter)
{
	debug("\tCounter : %u\n", (*counter));
}
