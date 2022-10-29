//
// Created by os on 8/9/22.
//

#include "../h/KSemaphore.h"

KSemaphore* KSemaphore::semaphores[NUMBER_OF_SEMAPHORES] = {};

void *KSemaphore::operator new(size_t size) {
    size = (size % MEM_BLOCK_SIZE == 0 ? size / MEM_BLOCK_SIZE : size / MEM_BLOCK_SIZE + 1);
    return MemoryAllocator::getInstance()->mem_alloc(size);
}

void KSemaphore::operator delete(void *ptr) {
    MemoryAllocator::getInstance()->mem_free(ptr);
}

int KSemaphore::addSemaphore(int init) {
    int i = 0;
    for (; i < NUMBER_OF_SEMAPHORES; i++) {
        if (!semaphores[i]) break;
    }

    if (i == NUMBER_OF_SEMAPHORES)
        return -1; // No free space

    semaphores[i] = new KSemaphore(init);
    return i;
}

int KSemaphore::removeSemaphore(int id) {
    if (id >= 0 && id < NUMBER_OF_SEMAPHORES && semaphores[id]) {
        while(semaphores[id]->unblock() != -1);
        semaphores[id]->handle->active = false;
        return 0;
    }
    return -1;
}

KSemaphore *KSemaphore::getSemaphore(int id) {
    if (id >= 0 && id < NUMBER_OF_SEMAPHORES)
        return semaphores[id];
    else
        return nullptr;
}

int KSemaphore::wait() {
    if (!handle->active) return -1;
    if (--val < 0) {
        block();
        TCB *old = TCB::running;
        TCB::running = Scheduler::get();
        TCB::contextSwitch(&old->context, &TCB::running->context);
        TCB::running->setBlocked(false);
        numOfBlocked--;
    }
    if(!handle->active) return -1;
    return 0;
}

void KSemaphore::block() {
    if (!blocked) {
        blocked = tail = TCB::running;
        TCB::running->semaphoreNext = nullptr;
    }
    else {
        tail = tail->semaphoreNext = TCB::running;
    }
    numOfBlocked++;
    TCB::running->setBlocked(true);
}

int KSemaphore::signal() {
    if (++val <= 0) {
        return unblock();
    }
    return 0;
}

int KSemaphore::unblock() {
    if (!blocked) return -1;
    Scheduler::put(blocked);
    blocked = blocked->semaphoreNext;
    if (!blocked) tail = nullptr;
    return 0;
}

void KSemaphore::freeSpace(int id) {
    if (id >= 0 && id < NUMBER_OF_SEMAPHORES && semaphores[id])
        semaphores[id] = nullptr;
}
