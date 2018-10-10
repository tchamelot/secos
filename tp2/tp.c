/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <intr.h>

extern info_t *info;

void bp_handler()
{
	// Replace Trampoline (I don't know if it is usefull)
	asm volatile ("pusha");

	// Core of handler
	debug("Breakpoint handler\n");

	// Return from interruption
	asm volatile ("popa");
	asm volatile ("leave");
	asm volatile ("iret");
}

void bp_trigger()
{
	asm volatile ("int3");
	debug("End of trigger\n");
}

void print_idt(idt_reg_t* idt)
{
	int_desc_t* entry;
	unsigned int i;
	for(i = 0; i < idt->limit/sizeof(int_desc_t);i++)
	{
		debug("Entry %d\n", i);
		entry = &idt->desc[i];
		debug("\toffset: %x\n", entry->offset_1 | 
				(entry->offset_2 << 16));
		debug("\tselector: %x\n", entry->selector);
		debug("\tist: %x\n", entry->ist);
		debug("\ttype: %x\n", entry->type);
		debug("\tdpl: %x\n", entry->dpl);	
	}
}

void tp()
{
	idt_reg_t* idt = NULL;
	get_idtr(*idt);
	idt->desc[3].offset_1 = (unsigned int)bp_handler & 0xffff;
	idt->desc[3].offset_2 = ((unsigned int)bp_handler >> 16) & 0xffff;
	bp_trigger();
}
