/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <segmem.h>
#include <string.h>

extern info_t *info;

gdt_reg_t gdt;
seg_desc_t gdt_entries[4];


void print_gdt(gdt_reg_t* gdt)
{
	unsigned int i = 0;
	unsigned int entry_size = sizeof(seg_desc_t);
	for(i = 0; i < (gdt->limit / entry_size); i++)
	{
		debug("Entry %d:\n", i);
		debug("\tBase : 0x%x\n",
			(gdt->desc[i].base_3 << 24) |  
			(gdt->desc[i].base_2 << 16) |
			gdt->desc[i].base_1);
		debug("\tLimit : 0x%x\n",
			(gdt->desc[i].limit_2 << 16) |
			gdt->desc[i].limit_1);
		debug("\tSegment type : 0x%x\n", gdt->desc[i].type);
		debug("\tDescriptor type : %x\n", gdt->desc[i].s);
		debug("\tPresent : %x\n",	gdt->desc[i].p);
		debug("\tPrivilege : %x\n",	gdt->desc[i].dpl);
		debug("\t%d bits\n", 16 * (1 +	gdt->desc[i].d));
		debug("\tGranularity %x\n",	gdt->desc[i].g);
	}
}	

void set_desc(seg_desc_t* desc, uint32_t base, uint32_t limit, 
				uint8_t type, uint8_t dpl,
				uint8_t d, uint8_t g)
{
	desc->base_1 = base & 0xffff;
	desc->base_2 = (base >> 16) & 0xff;
	desc->base_3 = (base >> 24) & 0xff;
	desc->limit_1 = limit & 0xffff;
	desc->limit_2 = (limit >> 16) & 0xf;
	desc->type = type & 0xf;
	if(type > 0)
	{
		desc->s = 1;
		desc->p = 1;
	}
	else
	{
		desc->s = 0;
		desc->p = 0;
	}
	desc->dpl = dpl & 0x3;
	desc->avl = 0;
	desc->l = 0;
	desc->d = d & 0x1;
	desc->g = g & 0x1;
}

void tp()
{
	char src[64];
	char *dst = 0;

	gdt_reg_t* gdt_ptr = NULL;
	get_gdtr(*gdt_ptr);
	print_gdt(gdt_ptr);	
	
	gdt.limit = 4 * sizeof(seg_desc_t) - 1;
	gdt.desc = gdt_entries;

	// First entry is NULL
	set_desc(&(gdt_entries[0]), 0x0, 0x0, 0, 0, 0, 0);
	
	// Second entry code, 32 bits, RX, flat
	set_desc(&(gdt_entries[1]), 0x0, 0xfffff, SEG_DESC_CODE_XR,
				0, 1, 1);

	// Third entry data, 32 bits, RW, flat
	set_desc(&(gdt_entries[2]), 0x0, 0xfffff, SEG_DESC_DATA_RW,
				0, 1, 1);
	
	// Fourth entry data, 32 bits, RW, base 0x600000, limite 32o
	set_desc(&(gdt_entries[3]), 0x600000, 0x20, 
				SEG_DESC_DATA_RW,
				0, 1, 0);

	set_gdtr(gdt);
	set_es(0x10);
	set_ss(0x10);
	set_ds(0x10);
	set_gs(0x10);
	set_fs(0x10);
	set_cs(0x08);

	memset(src, 'a', 64);

	set_es(0x18);

	_memcpy8(dst, src, 32);
}
