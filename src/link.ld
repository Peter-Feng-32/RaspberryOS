SECTIONS
{
    . = 0x80000;

	.text.boot : { *(.text.boot) }
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