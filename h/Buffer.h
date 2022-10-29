//
// Created by os on 8/20/22.
//

#ifndef PROJECT_BASE_1_1_BUFFER_H
#define PROJECT_BASE_1_1_BUFFER_H

#include "KSemaphore.h"


class Buffer {
public:
    Buffer(): head(0), tail(0), count(0), spaceAvailable(BUFFER_SIZE), itemAvailable(0) {}

    void *operator new(size_t size) {
        size = (size % MEM_BLOCK_SIZE == 0 ? size / MEM_BLOCK_SIZE : size / MEM_BLOCK_SIZE + 1);
        return MemoryAllocator::getInstance()->mem_alloc(size);
    }
    void operator delete(void *ptr) {
        MemoryAllocator::getInstance()->mem_free(ptr);
    }

    void putc(char c);
    char getc();

    int getCount() const {
        return count;
    }

private:
    static const int BUFFER_SIZE = 3;
    int head, tail, count;
    char buffer[BUFFER_SIZE];
    KSemaphore spaceAvailable, itemAvailable;
};


#endif //PROJECT_BASE_1_1_BUFFER_H
