ENTRY(rst00)
__stack_size = 0x100;

MEMORY
{
    RST (rx) : ORIGIN = 0x0000, LENGTH = 0x0040
    ROM (rx) : ORIGIN = 0x0040, LENGTH = 0x2000
    RAM (rwx) : ORIGIN = 0x2000, LENGTH = 0x2000
}

SECTIONS
{
    .rst : {
        *(.rst)
    } > RST

    .text : {
        *(.text)
    } > ROM

    .data : {
        *(.data)
    } > RAM AT > ROM

    .bss : {
        *(.bss)
    } > RAM
}

__data_load = LOADADDR(.data);
__data_start = ADDR(.data);
__data_size = SIZEOF(.data);

__sp_init = __data_start + __data_size + __stack_size;
