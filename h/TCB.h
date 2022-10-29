//
// Created by os on 6/25/22.
//

#ifndef PROJECT_BASE_1_1_TCB_H
#define PROJECT_BASE_1_1_TCB_H

#include "MemoryAllocator.h"
#include "Scheduler.h"


static const size_t STACK_SIZE_IN_BLOCKS = DEFAULT_STACK_SIZE / MEM_BLOCK_SIZE + (DEFAULT_STACK_SIZE % MEM_BLOCK_SIZE > 0 ? 1 : 0);
#define NUMBER_OF_THREADS 100

void idleFunction(void *arg);

class TCB {

public:

    ~TCB() {
        MemoryAllocator *ma = MemoryAllocator::getInstance();
        ma->mem_free(systemStack);
        ma->mem_free(userStack);
    }

    using Body = void(*)(void*);

    void *operator new(size_t size);
    void operator delete(void *ptr);

    static uint64 addThread(Body body, void *arg, void *stack_space);
    static int removeThread(uint64 id);
    static TCB *getThread(uint64 id);

    static TCB *running;

    static void changeToSystemStack();
    static void changeToUserStack();

    bool isFinished() const {
        return finished;
    }

    void setFinished(bool finished) {
        TCB::finished = finished;
    }

    static void setRunning(int id);

    static void dispatch();

    TCB(Body body, void *arg, void *stack_space) :
            body(body),
            arg(arg),
            systemStack(body ? (uint64*) (MemoryAllocator::getInstance())->mem_alloc(STACK_SIZE_IN_BLOCKS) : nullptr),
            systemSP(body ? (uint64*)((char*)systemStack + STACK_SIZE_IN_BLOCKS * MEM_BLOCK_SIZE) : nullptr),
            userStack((uint64*)stack_space),
            userSP(stack_space ? (uint64*) ((char*)stack_space + STACK_SIZE_IN_BLOCKS * MEM_BLOCK_SIZE) : nullptr),
            context({
                stack_space ? (uint64) &threadWrapper : (uint64) body,
                (uint64) (stack_space ? ((uint64 *)userSP - 32) : ((uint64 *)systemSP - 32))
                    }),
            timeSlice(DEFAULT_TIME_SLICE),
            finished(false),
            schedulerNext(nullptr),
            semaphoreNext(nullptr),
            sleepNext(nullptr),
            id(-1),
            blocked(false)
    {
        if (stack_space) Scheduler::put(this);
    }

    int getID() {
        return id;
    }

    void setID(int id) {
        this->id = id;
    }

    bool isBlocked() const {
        return blocked;
    }

    void setBlocked(bool b) {
        blocked = b;
    }


private:
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    void *arg;

    uint64 *systemStack;// Used for system stack allocation, points at top of system stack
    uint64 *systemSP;   // Used for saving the current value of system stack pointer
    uint64 *userStack;  // Used for user stack allocation, points at top of user stack
    uint64 *userSP;     // Used for saving the current value of user stack pointer

    Context context;
    uint64 timeSlice;
    bool finished;
    TCB *schedulerNext; // Pointer at next thread in scheduling queue
    TCB *semaphoreNext; // Pointer at next thread in semaphore queue
    TCB *sleepNext;     // Pointer at next thread in sleeping queue
    time_t sleepTime;

    static uint64 timeSliceCounter;

    static void threadWrapper();

    friend class Scheduler;
    friend class Riscv;
    friend class KSemaphore;
    friend class Sleep;
    friend class KConsole;

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static TCB *threads[NUMBER_OF_THREADS];

    int id;
    bool blocked;
};

#endif //PROJECT_BASE_1_1_TCB_H
