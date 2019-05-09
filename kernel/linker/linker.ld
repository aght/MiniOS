
MEMORY
{
	ram : ORIGIN = 0x8000, LENGTH = 0x8000000 /* 0x10000000=256MB Half of that is 125MB=0x8000000 */
}																						

__stack_size__ = DEFINED(__stack_size__) ? __stack_size__ : 0x100000; 

SECTIONS
{
   . = 0x8000;
    __start = .;

    .text : 
    {
        __text_start = .;
        KEEP(*(.boot))
        *(.text)
        __text_end = .;
    } > ram

    .data :
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data)
        __data_end__ = .;
    } > ram

    .bss (NOLOAD):
    {
       . = ALIGN(4);
        __bss_start__ = .;
        *(.bss)
        __bss_end__ = .;
    } > ram

    .rodata :
    {
        . = ALIGN(4);
        __rodata_start = .;
        *(.rodata)
        __rodata_end = .;
    } > ram

    .stack (NOLOAD):
    {
        . = ALIGN(8);
        __stack_start = .;
        . = . + __stack_size__;
        __stack_end = .;
    } > ram

    .heap :
    {
        . = ALIGN(4);
        __heap_start__ = .;
        *(.heap)
        _end = .; PROVIDE (end = .); /* Any memory from here is free to use so this is end of code and start of heap */
    } > ram

    /* Remove information from the standard libraries */
    /DISCARD/ :
    {
        /* We cant debug */

        /* so no point to hold arm unwind tables */
        *(.ARM.exidx .ARM.exidx.*)
        *(.rel.ARM.exidx .rel.ARM.exidx.* .gnu.linkonce.armexidx.*)

         /* same with debug frames */
        *(.debug_frame .debug_frame.*)
       
        /* We really don't need comments from compiler either  */
        *(.comment *.comment.*) 

        /* We really don't need the symbols and string tables  */
        *(.symtab .symtab.*)
        *(.strtab .strtab.*)
        *(.shstrtab .shstrtab.*)

        /* remove any standard library hang over bits */
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
        *(.note.GNU-stack)
        *(.gnu_debuglink) 
        *(.gnu.lto_*)
    }

	__end = .;
}
