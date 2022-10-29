//
// Created by os on 6/25/22.
//

#include "../h/TCB.h"
#include "../h/Riscv.h"
#include "../h/syscall_c.h"
#include "../h/print.h"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB* TCB::threads[NUMBER_OF_THREADS] = {};

void TCB::setRunning(int id) {
    if (id > 0 && id < NUMBER_OF_THREADS && threads[id])
        running = threads[id];
}

void *TCB::operator new(size_t size) {
    size = (size % MEM_BLOCK_SIZE == 0 ? size / MEM_BLOCK_SIZE : size / MEM_BLOCK_SIZE + 1);
    return MemoryAllocator::getInstance()->mem_alloc(size);
}

void TCB::operator delete(void *ptr) {
    MemoryAllocator::getInstance()->mem_free(ptr);
}



void TCB::changeToSystemStack() {
        if (running->body) {
            __asm__ volatile("sd sp, 0(%[userSP])" : : [userSP] "r"(&running->userSP));
            __asm__ volatile("ld sp, 0(%[systemSP])" : : [systemSP] "r"(&running->systemSP));
            // decrement of sp by 16 because it will increment by 16 after returning from this function (in order to pop the s0 register used as base pointer)
            __asm__ volatile("addi sp, sp, -16");
        }
}

void TCB::changeToUserStack() {
    if (running->body) {
        __asm__ volatile("ld sp, 0(%[userSP])" : : [userSP] "r"(&running->userSP));
        // decrement of sp by 16 because it will increment by 16 after returning from this function (in order to pop the s0 register used as base pointer)
        //__asm__ volatile("addi sp, sp, -16");
    }
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

void TCB::dispatch() {
    TCB *old = running;
    if (!old->isFinished() && old->body != &idleFunction) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

uint64 TCB::addThread(TCB::Body body, void *arg, void *stack_space) {
    int i = 0;
    for (; i < NUMBER_OF_THREADS; i++) {
        if (!threads[i]) break;
    }

    if (i == NUMBER_OF_THREADS)
        return -1; // No free space

    threads[i] = new TCB(body, arg, stack_space);
    threads[i]->setID(i);
    return i;
}

int TCB::removeThread(uint64 id) {
    if (id >= 0 && id < NUMBER_OF_THREADS && threads[id]) {
        //delete threads[id];
        //threads[id] = nullptr;
        return 0;
    }
    else
        return -1;
}

TCB *TCB::getThread(uint64 id) {
    if (id >= 0 && id < NUMBER_OF_THREADS && threads[id])
        return threads[id];
    else
        return nullptr;
}



