//
// Created by os on 5/25/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_H
#define PROJECT_BASE_MEMORYALLOCATOR_H

#include "../lib/hw.h"


class MemoryAllocator {
public:
    static MemoryAllocator* getInstance();

    void* mem_alloc(uint64 size);

    int mem_free(void* ptr);

    struct FreeMem {
        FreeMem *next, *prev;
        uint64 size; //Size of free space in number of blocks
    };

protected:
    MemoryAllocator() :freeMemHead(nullptr) {}
    int tryToJoin(FreeMem*);

private:
    static MemoryAllocator* _instance;

    FreeMem* freeMemHead; //Descriptor(header) of free memory space on heap
};


#endif //PROJECT_BASE_MEMORYALLOCATOR_H
