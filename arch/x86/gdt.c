#include <stdint.h>
#include "gdt.h"

/* 
   The GDT will contain 3 entries:
   0: Null descriptor
   1: Kernel code segment
   2: Kernel data segment
*/

#define GDT_ENTRIES 3

/* GDT entry structure array */
struct gdt_entry gdt[GDT_ENTRIES];

/* Pointer structure used by LGDT */
struct gdt_ptr gp;


/* 
   Sets a single GDT entry
*/
void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    /* Base address */
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;

    /* Segment limit */
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    /* Granularity flags */
    gdt[num].granularity |= gran & 0xF0;

    /* Access flags */
    gdt[num].access = access;
}


/*
   Initialize the Global Descriptor Table
*/
void gdt_init()
{
    /* Setup the GDT pointer */
    gp.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gp.base  = (uint32_t)&gdt;

    /* Null segment (required first entry) */
    gdt_set_entry(0, 0, 0, 0, 0);

    /* Kernel Code Segment */
    gdt_set_entry(
        1,
        0,              // base
        0xFFFFFFFF,     // limit
        0x9A,           // access
        0xCF            // granularity
    );

    /* Kernel Data Segment */
    gdt_set_entry(
        2,
        0,
        0xFFFFFFFF,
        0x92,
        0xCF
    );

    /* Load the GDT using assembly */
    gdt_flush((uint32_t)&gp);
}