#include "heap.h"

static uint32_t heap_current;

void heap_init()
{
    heap_current = HEAP_START;
}

void* kmalloc(uint32_t size)
{
    uint32_t addr = heap_current;
    heap_current += size;

    return (void*)addr;
}

void kfree(void* ptr)
{
    /* Simple kernel heap does not support free yet */
    (void)ptr;
}