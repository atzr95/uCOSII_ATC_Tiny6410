MEMORY
{
  SRAM (wx) : ORIGIN = 0x50000000, LENGTH = 0x08000000
}


SECTIONS
{
  __SRAM_segment_start__ = 0x50000000;
  __SRAM_segment_end__ = 0x58000000;

  __HEAPSIZE__ = 67108864;
  __STACKSIZE_ABT__ = 0;
  __STACKSIZE_FIQ__ = 256;
  __STACKSIZE_IRQ__ = 256;
  __STACKSIZE_SVC__ = 0;
  __STACKSIZE_UND__ = 0;
  __STACKSIZE__ = 1024;

  __init_load_start__ = ALIGN(__SRAM_segment_start__ , 4);
  .init ALIGN(__SRAM_segment_start__ , 4) : AT(ALIGN(__SRAM_segment_start__ , 4))
  {
    __init_start__ = .;
    *(.init .init.* .fini)
  }
  __init_end__ = __init_start__ + SIZEOF(.init);

  __init_load_end__ = __init_end__;

  __text_load_start__ = ALIGN(__init_end__ , 4);
  .text ALIGN(__init_end__ , 4) : AT(ALIGN(__init_end__ , 4))
  {
    __text_start__ = .;
    *(.text .text.* .glue_7t .glue_7 .gnu.linkonce.t.* .gcc_except_table .ARM.extab* .gnu.linkonce.armextab.*)
  }
  __text_end__ = __text_start__ + SIZEOF(.text);

  __text_load_end__ = __text_end__;

  __dtors_load_start__ = ALIGN(__text_end__ , 4);
  .dtors ALIGN(__text_end__ , 4) : AT(ALIGN(__text_end__ , 4))
  {
    __dtors_start__ = .;
    KEEP (*(SORT(.dtors.*))) KEEP (*(.dtors)) KEEP (*(.fini_array .fini_array.*))
  }
  __dtors_end__ = __dtors_start__ + SIZEOF(.dtors);

  __dtors_load_end__ = __dtors_end__;

  __ctors_load_start__ = ALIGN(__dtors_end__ , 4);
  .ctors ALIGN(__dtors_end__ , 4) : AT(ALIGN(__dtors_end__ , 4))
  {
    __ctors_start__ = .;
    KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors)) KEEP (*(.init_array .init_array.*))
  }
  __ctors_end__ = __ctors_start__ + SIZEOF(.ctors);

  __ctors_load_end__ = __ctors_end__;

  __data_load_start__ = ALIGN(__ctors_end__ , 4);
  .data ALIGN(__ctors_end__ , 4) : AT(ALIGN(__ctors_end__ , 4))
  {
    __data_start__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
  }
  __data_end__ = __data_start__ + SIZEOF(.data);

  __data_load_end__ = __data_end__;

  __rodata_load_start__ = ALIGN(__data_end__ , 4);
  .rodata ALIGN(__data_end__ , 4) : AT(ALIGN(__data_end__ , 4))
  {
    __rodata_start__ = .;
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  }
  __rodata_end__ = __rodata_start__ + SIZEOF(.rodata);

  __rodata_load_end__ = __rodata_end__;

  __ARM.exidx_load_start__ = ALIGN(__rodata_end__ , 4);
  .ARM.exidx ALIGN(__rodata_end__ , 4) : AT(ALIGN(__rodata_end__ , 4))
  {
    __ARM.exidx_start__ = .;
    __exidx_start =   __ARM.exidx_start__;
    *(.ARM.exidx .ARM.exidx.*)
  }
  __ARM.exidx_end__ = __ARM.exidx_start__ + SIZEOF(.ARM.exidx);

  __exidx_end =   __ARM.exidx_end__;
  __ARM.exidx_load_end__ = __ARM.exidx_end__;

  __bss_load_start__ = ALIGN(__ARM.exidx_end__ , 4);
  .bss ALIGN(__ARM.exidx_end__ , 4) (NOLOAD) : AT(ALIGN(__ARM.exidx_end__ , 4))
  {
    __bss_start__ = .;
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  }
  __bss_end__ = __bss_start__ + SIZEOF(.bss);

  __bss_load_end__ = __bss_end__;

  __non_init_load_start__ = ALIGN(__bss_end__ , 4);
  .non_init ALIGN(__bss_end__ , 4) (NOLOAD) : AT(ALIGN(__bss_end__ , 4))
  {
    __non_init_start__ = .;
    *(.non_init .non_init.*)
  }
  __non_init_end__ = __non_init_start__ + SIZEOF(.non_init);

  __non_init_load_end__ = __non_init_end__;

  __tbss_load_start__ = ALIGN(__non_init_end__ , 4);
  .tbss ALIGN(__non_init_end__ , 4) (NOLOAD) : AT(ALIGN(__non_init_end__ , 4))
  {
    __tbss_start__ = .;
    *(.tbss .tbss.*)
  }
  __tbss_end__ = __tbss_start__ + SIZEOF(.tbss);

  __tbss_load_end__ = __tbss_end__;

  __SRAM_segment_used_end__ = ALIGN(__non_init_end__ , 4) + SIZEOF(.tbss);

  heap_start = 0x53000000;
  heap_end = heap_start + __HEAPSIZE__;
}

