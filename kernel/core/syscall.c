#include <debug.h>
#include <pagemem.h>
#include <intr.h>
#include <cr.h>
#include <syscall.h>
#include <task.h>

void (*syscall[])(uint32_t*) = {
	print_counter_hdl
};

void syscall_hdl(int_ctx_t* ctx)
{
	uint32_t nb_sys = ctx->gpr.eax.raw;
	if(nb_sys < sizeof(syscall))
	{
		syscall[nb_sys]((uint32_t*)ctx->gpr.ebx.raw);
	}
}

void __printer__ print_counter(uint32_t* counter)
{
	asm volatile("int $0x80"::"a"(0x0),"b"(counter));
}

void print_counter_hdl(uint32_t* counter)
{	
	if(check_arg_ptr(counter))
		debug("Counter : %u\n", (*counter));
	else
		debug("[Syscall] Error bad pointer\n");
}

uint32_t check_arg_ptr(uint32_t* arg)
{
	uint32_t valid = 0;

	pde32_t* pdg = (pde32_t*)get_cr3();
	pte32_t* pte = (pte32_t*)(pdg[pd32_idx(arg)].addr << 12);

	if((pdg[pd32_idx(arg)].raw & (PG_USR | PG_P)) == (PG_USR | PG_P))
		if((pte[pt32_idx(arg)].raw & (PG_USR | PG_P)) == (PG_USR | PG_P))
			valid = 1;
	
	return valid;
}
