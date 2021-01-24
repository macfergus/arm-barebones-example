MEMORY
{
    /* Flash goes up to 0x80000, but the first 0x4000 hold the bootloader.
     * For a true bare-metal build (no bootloader), you'd skip the BOOT part and
     * have FLASH start as 0x0 instead */
    BOOT (r) : ORIGIN = 0x00000000, LENGTH = 0x00004000
    FLASH (rx) : ORIGIN = 0x00004000, LENGTH = 0x80000-0x4000

    /* SRAM starts at 0x2000000 and there's 192KB */
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x30000
}

ENTRY(resetHandler)

SECTIONS
{
    .text :
    {
        . = ALIGN(0x4000);
        /* The ISR table MUST live here
         * For a true bare-metal setup (no bootloader), you'd start at 0x0000
         * instead of 0x4000 */
        KEEP(*(.isr_vector));
        *(.text*)
    } > FLASH

    /* A normal linker script will have many more sections: .data, .bss, etc */

    /* The stack starts at the end of memory and grows backward.
     * In our case, we allocate all of SRAM to the stack: the code executes
     * out of flash, and we don't have any globals or heap-allocated data */
    __stack_top = ORIGIN(RAM) + LENGTH(RAM);
}
