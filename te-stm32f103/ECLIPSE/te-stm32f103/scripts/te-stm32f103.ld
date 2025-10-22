OUTPUT_FORMAT ("elf32-littlearm")

MEMORY
{
  flash   (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
  ram    (rwx)  : ORIGIN = 0x20000000,   LENGTH = 64K
}

/* define a global symbol _stack_end  */
_stack_end_ = ORIGIN(ram) + LENGTH(ram) - 1 ;

/* now define the output sections  */
SECTIONS 
{
	. = 0;								/* set location counter to address zero  */
	.text :								
	{
		_image_start_  = . ;
		_vec_start_ = . ;
		KEEP(*(.flash_vec_table*))
		_vec_end_ = ALIGN( . , 8) ;
		
		_text_start_ = _vec_end_ ;
		/* вызов статических конструкторов */
		__preinit_array_start = . ;
		KEEP(*(.preinit_array*))
		__preinit_array_end = . ;
		__init_array_start  = . ;
		KEEP(*(.init_array*))
		__init_array_end    = . ;
		
		*(.text)
		*(.text*)						/* all .text sections (code)  */
		*(.rodata)						/* all .rodata sections (constants, strings, etc.)  */
		*(.rodata*)						/* all .rodata* sections (constants, strings, etc.)  */
		*(.glue_7)						/* all .glue_7 sections  (no idea what these are) */
		*(.glue_7t)						/* all .glue_7t sections (no idea what these are) */
		*(.gnu*)
		*(.gcc*)
		
		/*    не используется во встраиваемых системах
		__fini_array_start = . ;
		KEEP(*(.fini_array*))
		__fini_array_end   = . ;
		*/
               
		. = ALIGN(8) ;
		_text_end_ = .;					/* define a global symbol _etext just after the last code byte */
	} >flash							/* put all the above into FLASH */

	.bss :								 
	{
		_ram_vec_start_ = .;
		*(.ram_vec_table)                  /* area for STM32 in-RAM mapped IRQ vector table, not need initialize in startup*/
		*(.ram_vec_table*)			
		_ram_vec_end_ = ALIGN( . , 8) ;

		_bss_start_ = _ram_vec_end_ ;       
		*(.bss)			    /*initialize in startup for zero fill of gloabal objects*/			
		*(.bss*)
		_bss_end_ = .;
          
	}  >ram

	_data_load_start_ = _text_end_ ;
	.data :								/* collect all initialized .data sections that go into RAM  */ 
	{
		_data_start_ = .;						/* create a global symbol marking the start of the .data section  */
		*(.data)						/* all .data sections  */
		*(.data*)
		_data_end_ = ALIGN( . , 8);							/* define a global symbol marking the end of the .data section  */
	} >ram AT >flash					/* put all the above into RAM (but load the LMA copy into FLASH) */
	_data_load_end_ = _data_load_start_ +  SIZEOF(.data) ;	
   
	_fast_load_start_ = _data_load_end_ ;
	.fast :
	{
		_fast_start_ = .;
		*(.fast)
		*(.fast*)
		_fast_end_ = ALIGN( . , 8);
	} >ram AT >flash
	_fast_load_end_ = _fast_load_start_ +  SIZEOF(.fast) ;
	_image_end_ = _fast_load_end_ ;
  
   DISCARD :
    {
     libc.a ( * )
     libm.a ( * )
     libgcc.a ( * )
    }	
     
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
