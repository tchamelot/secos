/* GPLv2 (c) Airbus */
#include <debug.h>
#include <info.h>
#include <grub_mbi.h>
extern info_t   *info;
extern uint32_t __kernel_start__;
extern uint32_t __kernel_end__;

void tp()
{
   multiboot_memory_map_t *mmap, *end;

   debug("kernel mem [0x%x - 0x%x]\n", &__kernel_start__, &__kernel_end__);
   debug("MBI flags 0x%x\n", info->mbi->flags);
   debug("Memory map address\n");
   mmap = (multiboot_memory_map_t*)info->mbi->mmap_addr;
   end = (multiboot_memory_map_t*)(info->mbi->mmap_addr + info->mbi->mmap_length);
   for(;mmap < end; mmap++)
   {
	debug("Base : %llx, len : %llx, type %lx \n", mmap->addr, mmap->len, mmap->type);
   }
   int *ptr = (int*)0x100000;
   *ptr = 5;
   debug("ptr = %d\n", *ptr);
}
