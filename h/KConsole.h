//
// Created by os on 8/19/22.
//

#ifndef PROJECT_BASE_1_1_KCONSOLE_H
#define PROJECT_BASE_1_1_KCONSOLE_H

#include "Buffer.h"

class KConsole {
public:

    KConsole(): inputBuffer(new Buffer()), outputBuffer(new Buffer()),
                printThread(new TCB(&KConsole::print, nullptr, nullptr)),
                scanThread(new TCB(&KConsole::scan, nullptr, nullptr))
                {}

    static KConsole* getInstance() {
        if (!instance) {
            instance = new KConsole();
        }
        return instance;
    }

    Buffer *getInputBuffer() const {
        return inputBuffer;
    }

    Buffer *getOutputBuffer() const {
        return outputBuffer;
    }

    TCB *getPrintThread() const {
        return printThread;
    }

    TCB *getScanThread() const {
        return scanThread;
    }

    void *operator new(size_t size) {
        size = (size % MEM_BLOCK_SIZE == 0 ? size / MEM_BLOCK_SIZE : size / MEM_BLOCK_SIZE + 1);
        return MemoryAllocator::getInstance()->mem_alloc(size);
    }
    void operator delete(void *ptr) {
        MemoryAllocator::getInstance()->mem_free(ptr);
    }

private:
    static KConsole *instance;

    Buffer *inputBuffer, *outputBuffer;
    TCB *printThread, *scanThread;

    static void print(void *arg);
    static void scan(void *arg);


};


#endif //PROJECT_BASE_1_1_KCONSOLE_H
