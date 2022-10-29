//
// Created by os on 5/25/22.
//

#include "../h/MemoryAllocator.h"

int ma[sizeof(MemoryAllocator) / sizeof(int) + 1];

MemoryAllocator* MemoryAllocator::_instance = nullptr;

MemoryAllocator *MemoryAllocator::getInstance() {
    if (_instance == nullptr) {
        _instance = (MemoryAllocator*) ma;
        _instance->freeMemHead = (FreeMem*) HEAP_START_ADDR;
        _instance->freeMemHead->next = _instance->freeMemHead->prev = nullptr;
        _instance->freeMemHead->size = ((uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR - MEM_BLOCK_SIZE) / MEM_BLOCK_SIZE;
    }
    return _instance;
}

void *MemoryAllocator::mem_alloc(uint64 size) {
    FreeMem *curr = freeMemHead;

    for (; curr && curr->size < size; curr = curr->next);

    if (!curr) return nullptr;

    if (curr->size - size > 1) {

        //If new free memory should be added in list
        FreeMem *newFreeMem = (FreeMem*)((char*)curr + (size + 1) * MEM_BLOCK_SIZE);
        newFreeMem->size = curr->size - (size + 1);
        curr->size = size;

        newFreeMem->next = curr->next;
        newFreeMem->prev = curr->prev;
        if(curr->next) curr->next->prev = newFreeMem;
        if(curr->prev) curr->prev->next = newFreeMem;
        else freeMemHead = newFreeMem;

    }
    else {
        if (curr->next) curr->next->prev = curr->prev;
        if (curr->prev) curr->prev->next = curr->next;
        else freeMemHead = curr->next;
    }

    curr->next = curr->prev = nullptr;
    return (char*)curr + MEM_BLOCK_SIZE;
}

int MemoryAllocator::mem_free(void *ptr) {
    if (!ptr) return -1;
    ptr = (char*)ptr - MEM_BLOCK_SIZE;
    // Find the place where to insert the new free segment (just after curr):
    FreeMem* curr = nullptr;
    if (!freeMemHead || ptr < (char*)freeMemHead)
        curr = nullptr; // insert as the first
    else
        for (curr = freeMemHead; curr->next != nullptr && ptr > (char*)(curr->next); curr = curr->next);

    // Insert the new segment after curr:
    FreeMem* newFreeMem = (FreeMem*)ptr;
    newFreeMem->prev = curr;
    if (curr) newFreeMem->next = curr->next;
    else newFreeMem->next = freeMemHead;


    if (newFreeMem->next) newFreeMem->next->prev = newFreeMem;
    if (curr) curr->next = newFreeMem;
    else freeMemHead = newFreeMem;

    // Try to merge with the previous and next segments:
    tryToJoin(newFreeMem);
    tryToJoin(curr);

    return 0;
}

int MemoryAllocator::tryToJoin(MemoryAllocator::FreeMem *curr) {
    if (!curr) return 0;
    if (curr->next && (char*)curr + MEM_BLOCK_SIZE + curr->size * MEM_BLOCK_SIZE == (char*)(curr->next)) {
        // Remove the cur->next segment:
        curr->size += curr->next->size + 1;
        curr->next = curr->next->next;
        if (curr->next) curr->next->prev = curr;
        return 1;
    } else
        return 0;
}

