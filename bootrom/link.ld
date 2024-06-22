ENTRY(rst00)

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
