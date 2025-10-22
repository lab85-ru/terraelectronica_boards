
ENTRY(_vectors)

MEMORY
{
  ROM(RX):  ORIGIN = 0x00000000, LENGTH = 512K
  RAM(RW):  ORIGIN = 0x40000000, LENGTH = 64K
}

SECTIONS
{
  .text :
  {
    _text_start = .;
    KEEP(*(.vect))
    . = ALIGN(4);
    . = 0x000001FC;
    KEEP(*(.crp))
    KEEP(*(.init))
    *(.text .text.*)
    *(.rodata)
    *(.rodata*)
    *(.glue_7)
    *(.glue_7t)
  } > ROM
  . = ALIGN(4);
  .ctors :
  {
    _ctors_start = .;
    KEEP(*(SORT(.ctors.*)))
    KEEP(*(.ctors))
    _ctors_end = .;
  } > ROM
  .dtors :
  {
    _dtors_start = .;
    KEEP(*(SORT(.dtors.*)))
    KEEP(*(.dtors))
    _dtors_end = .;
  } > ROM
  . = ALIGN(4);
  _text_end = .;
  PROVIDE(etext = .);

  .data : AT(_text_end)
  {
    _data_start = .;
    *(.data)
    SORT(CONSTRUCTORS)
    *(.ramfunc)
  } > RAM
  . = ALIGN(4);
  _data_end = .;
  PROVIDE(edata = .);

  .bss(NOLOAD) :
  {
    _bss_start = .;
    *(.bss)
    *(COMMON)
  } > RAM
  . = ALIGN(4);
  _bss_end = .;

  .stack ALIGN(256):
  {
    *(.stack)
    PROVIDE(_stack = .);
  } > RAM

  _end = .;
  PROVIDE( end = .);

  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}

