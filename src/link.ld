SECTIONS
{
    . = 0x200000;

	.text.init : { *(.text.init) }
	.text : { *(.text .text.*) }
	.rodata : { *(.rodata .rodata.*) }
	PROVIDE(_data = .);

	.data : { *(.data) }
    
	. = ALIGN(0x8); 
	bss_begin = .;
	.bss : { *(.bss*) } 
	bss_end = .;
    
    /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }

}