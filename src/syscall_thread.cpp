//
// Created by os on 6/27/22.
//

using Body = void(*)(void*);
#include "../h/TCB.h"
#include "../h/Riscv.h"
#include "../h/handles.h"
#include "../h/Sleep.h"

int sys_thread_create() {

    uint64 *sscratch = (uint64 *)(Riscv::r_sscratch());

    thread_t* handle = (thread_t *)(*sscratch);
    Body body = (Body)(*(sscratch + 1));
    void *arg = (void *)(*(sscratch + 2));
    void *stack_space = (void *)(*(sscratch + 3));

    *handle = (thread_data *)MemoryAllocator::getInstance()->mem_alloc(sizeof(thread_data));

    (*handle)->id = TCB::addThread(body, arg, stack_space);
    if ((*handle)->id >= 0)
        return 0;
    else
        return -1;
}

int sys_thread_exit() {
    TCB::running->setFinished(true);
    int id = TCB::running->getID();
    return TCB::removeThread(id);
}

int sys_time_sleep() {
    uint64 *sscratch = (uint64 *)(Riscv::r_sscratch());
    time_t timeSleep = (time_t)(*sscratch);

    return Sleep::sleep(timeSleep);
}
