/* GPLv2 (c) Airbus */
OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386");
OUTPUT_ARCH("i386")

ENTRY(entry)

PHDRS
{
   phboot  PT_LOAD FLAGS (7);
   phstack PT_LOAD FLAGS (6);
   phsetup PT_LOAD FLAGS (7);
   phucode PT_LOAD FLAGS (7);
   /*phustack1 PT_LOAD FLAGS (6);*/
   /*phucode2 PT_LOAD FLAGS (7);*/
   /*phustack2 PT_LOAD FLAGS (6);*/
}

SECTIONS
{
   . = 0x300000;
   .mbh      : { KEEP(*(.mbh)) . = ALIGN(4);     } : phboot
   .stack    : { KEEP(*(.stack))                 } : phstack

   __kernel_start__ = .;

   .idt_jmp  : { KEEP(*(.idt_jmp))               } : phsetup
   .text     : { *(.text)                        } : phsetup
   .rodata   : { *(.rodata)                      } : phsetup
   .data     : { *(.data)                        } : phsetup
   .bss      : { *(.bss COMMON)                  } : phsetup
   /DISCARD/ : { *(.note* .indent .comment)      } : phsetup

   __kernel_end__ = .;

   . = 0x800000;
   __user_start__ = .;
   /*.ustack1  : { KEEP(*(.ustack1))		 } : phustack1	*/
   /*. = 0x801000;						*/
   /*.kstack1  : { KEEP(*(.kstack1))		 } : phustack1	*/
   . = 0x802000;
   .ucode    : { KEEP(*(.ucode))		 } : phucode
   __user_end__ = .;

  /*
   . = 0xc00000;
   __user2_start__ = .;
   .ustack2  : { KEEP(*(.ustack2))		 } : phustack2
   . = 0xc01000;
   .kstack2  : { KEEP(*(.kstack2))		 } : phustack2
   . = 0xc02000;
   .ucode2    : { KEEP(*(.ucode2))		 } : phucode2
   __user1_end__ = .;
  */
}
