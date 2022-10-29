//
// Created by os on 6/21/22.
//
#include "../h/MemoryAllocator.h"

void *sys_mem_alloc(void) {
    size_t size;
    __asm__ volatile("mv %0, a1" : "=r" (size));
    MemoryAllocator *instance = MemoryAllocator::getInstance();
    return instance->mem_alloc(size);
}

int sys_mem_free(void) {
    void *ptr;
    __asm__ volatile("mv %0, a1" : "=r" (ptr));
    MemoryAllocator *instance = MemoryAllocator::getInstance();
    return instance->mem_free(ptr);
}