//
// Created by os on 8/9/22.
//

#ifndef PROJECT_BASE_1_1_KSEMAPHORE_H
#define PROJECT_BASE_1_1_KSEMAPHORE_H

#define NUMBER_OF_SEMAPHORES 100
#include "TCB.h"
#include "handles.h"

class KSemaphore {
public:

    KSemaphore(int init = 1) : val(init), blocked(nullptr), tail(nullptr), numOfBlocked(0) {
        handle = (semaphore_data*)MemoryAllocator::getInstance()->mem_alloc(sizeof(semaphore_data));
        handle->active = true;
        handle->id = -1;
    }

    static int addSemaphore(int init);
    static int removeSemaphore(int id);
    static KSemaphore *getSemaphore(int id);

    int wait();
    int signal();

    bool isActive() { return handle->active; }
    bool haveBlocked() { return numOfBlocked > 0; }

    static void freeSpace(int id);

private:
    int val;
    TCB* blocked;
    TCB* tail;
    static KSemaphore *semaphores[NUMBER_OF_SEMAPHORES];
    sem_t handle;
    int numOfBlocked;

    void *operator new(size_t size);
    void operator delete(void *ptr);

    void block();
    int unblock();
};


#endif //PROJECT_BASE_1_1_KSEMAPHORE_H
