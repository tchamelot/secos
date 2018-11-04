#include <debug.h>
#include <segmem.h>
#include "seg_init.h"

//TODO select specific value for gdt et gdtr

seg_desc_t GDT[7];
tss_t TSS1;
tss_t TSS2;

void init_segmem(void)
{
	gdt_reg_t gdtr;
	GDT[0].raw = 0ULL;	// First entry NULL
	c0_dsc(&GDT[1]);	// second entry code r0
	d0_dsc(&GDT[2]);	// Third entry data r0
	c3_dsc(&GDT[3]);	// Fourth entry code r3
	d3_dsc(&GDT[4]);	// Fifth entry data r3
	tss_dsc(&GDT[5], (offset_t)&TSS1);

	gdtr.desc = GDT;
	gdtr.limit = sizeof(GDT) - 1;
	debug("\tsetting gdtr\n");
	set_gdtr(gdtr);		// Apply new gdt

	debug("\tflushing segments\n");
	set_cs(c0_sel);		// Flush segment registers
	set_ss(d0_sel);
	set_ds(d0_sel);
	set_es(d0_sel);
	set_fs(d0_sel);
	set_gs(d0_sel);
}
