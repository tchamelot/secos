/* GPLv2 (c) Airbus */
#include <intr.h>
#include <debug.h>
#include <info.h>
#include <scheduler.h>
#include <syscall.h>

extern info_t *info;
extern void idt_trampoline();
static int_desc_t IDT[IDT_NR_DESC];

void intr_init()
{
   idt_reg_t idtr;
   offset_t  isr;
   size_t    i;

   isr = (offset_t)idt_trampoline;

   /* re-use default grub GDT code descriptor */
   for(i=0 ; i<IDT_NR_DESC ; i++, isr += IDT_ISR_ALGN)
	   int_desc(&IDT[i], gdt_krn_seg_sel(1), isr);
   
   IDT[0x80].dpl = 3;

   idtr.desc  = IDT;
   idtr.limit = sizeof(IDT) - 1;
   set_idtr(idtr);
}

void __regparm__(1) intr_hdlr(int_ctx_t *ctx)
{
	uint8_t vector = ctx->nr.blow;

	switch(vector)
	{
		case 32:
			scheduler_hdl(ctx);
			break;
		case 128:
			syscall_hdl(ctx);
			break;
		default:
			if(vector < NR_EXCP)
      				excp_hdlr(ctx);
   			else
      				debug("ignore IRQ %d\n", vector);
   }
}
