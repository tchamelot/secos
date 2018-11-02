/* GPLv2 (c) Airbus */
#include <debug.h>
#include <segmem.h>
#include <string.h>
#include <info.h>
#include <asm.h>
#include <intr.h>

extern info_t *info;

#define c0_idx  1
#define d0_idx  2
#define c3_idx  3
#define d3_idx  4
#define tss_idx 5

#define c0_sel  gdt_krn_seg_sel(c0_idx)
#define d0_sel  gdt_krn_seg_sel(d0_idx)
#define c3_sel  gdt_usr_seg_sel(c3_idx)
#define d3_sel  gdt_usr_seg_sel(d3_idx)
#define ts_sel  gdt_krn_seg_sel(tss_idx)

seg_desc_t GDT[6];
tss_t TSS;

#define gdt_flat_dsc(_dSc_,_pVl_,_tYp_) \
   ({                                   \
      (_dSc_)->raw     = 0;             \
      (_dSc_)->limit_1 = 0xffff;        \
      (_dSc_)->limit_2 = 0xf;           \
      (_dSc_)->type    = _tYp_;         \
      (_dSc_)->dpl     = _pVl_;         \
      (_dSc_)->d       = 1;             \
      (_dSc_)->g       = 1;             \
      (_dSc_)->s       = 1;             \
      (_dSc_)->p       = 1;             \
   })

#define tss_dsc(_dSc_,_tSs_)			\
   ({							\
      (_dSc_)->raw     = 0;				\
      (_dSc_)->base_1  = _tSs_ & 0xffff;		\
      (_dSc_)->base_2  = (_tSs_ >> 16) & 0xff;		\
      (_dSc_)->base_3  = (_tSs_ >> 24) & 0xff;		\
      (_dSc_)->limit_1 = sizeof(tss_t) & 0xffff;	\
      (_dSc_)->limit_2 = (sizeof(tss_t) >> 16) & 0xf;	\
      (_dSc_)->type    = SEG_DESC_SYS_TSS_AVL_32;	\
      (_dSc_)->p       = 1;				\
   })


#define c0_dsc(_d) gdt_flat_dsc(_d,0,SEG_DESC_CODE_XR)
#define d0_dsc(_d) gdt_flat_dsc(_d,0,SEG_DESC_DATA_RW)
#define c3_dsc(_d) gdt_flat_dsc(_d,3,SEG_DESC_CODE_XR)
#define d3_dsc(_d) gdt_flat_dsc(_d,3,SEG_DESC_DATA_RW)

void show_gdt()
{
   gdt_reg_t gdtr;
   size_t    i,n;

   get_gdtr(gdtr);
   n = (gdtr.limit+1)/sizeof(seg_desc_t);
   for(i=0 ; i<n ; i++) {
      seg_desc_t *dsc   = &gdtr.desc[i];
      uint32_t    base  = dsc->base_3<<24 | dsc->base_2<<16 | dsc->base_1;
      uint32_t    limit = dsc->limit_2<<16| dsc->limit_1;
      debug("GDT[%d] = 0x%llx | base 0x%x | limit 0x%x | type 0x%x\n",
            i, gdtr.desc[i].raw, base, limit, dsc->type);
   }
   debug("--\n");
}

void init_gdt()
{
   gdt_reg_t gdtr;

   GDT[0].raw = 0ULL;

   c0_dsc( &GDT[c0_idx] );
   d0_dsc( &GDT[d0_idx] );
   c3_dsc( &GDT[c3_idx] );
   d3_dsc( &GDT[d3_idx] );

   gdtr.desc  = GDT;
   gdtr.limit = sizeof(GDT) - 1;
   set_gdtr(gdtr);

   set_cs(c0_sel);

   set_ss(d0_sel);
   set_ds(d0_sel);
   set_es(d0_sel);
   set_fs(d0_sel);
   set_gs(d0_sel);
}

void syscall_isr()
{
	asm volatile ("leave");
	asm volatile ("pusha");
	asm volatile ("mov %esp, %eax");
	asm volatile ("call syscall_handler");
	asm volatile ("popa");
	asm volatile ("iret");
}

void __regparm__(1) syscall_handler(int_ctx_t *ctx)
{
	debug("SYSCALL eax = %p\n", ctx->gpr.eax);
	debug("print syscall = %s\n", ctx->gpr.esi);
}

void init_syscall()
{
	idt_reg_t idt;
	get_idtr(idt);
	idt.desc[48].offset_1 = (unsigned int)syscall_isr & 0xffff;
	idt.desc[48].offset_2 = ((unsigned int)syscall_isr >> 16) & 0xffff;
	idt.desc[48].dpl = 3;
	//idt.desc[48].selector = c0_sel;
}

void userland()
{
	int i;
	debug("Userland\n");
	for(i = 0; i < 10; i++)
		debug("\t%d\n", i);

	asm volatile ("int $0x30"::"S"(0x304b64));
	//asm volatile ("int $0x30"::"S"("salut\n"));
	debug("End of userland\n");
	while(1);	
}

void tp()
{
   init_gdt();
   init_syscall();
   //force_interrupts_on();
   debug("Getting in userland\n");
   
   TSS.s0.esp = get_ebp();
   TSS.s0.ss = d0_sel; 
   tss_dsc( &GDT[tss_idx], (offset_t)&TSS );
   set_tr(ts_sel);

   asm volatile ("push %0    \n" // ss
		 "push %%ebp \n" // esp
		 "pushf      \n" // eflags
		 "push %1    \n" // cs
		 "push %2    \n" // eip
		 "iret"
		 ::
		 "i"(d3_sel),
		 "i"(c3_sel),
		 "r"(&userland));
}
