//
// Created by os on 8/9/22.
//

#include "../h/Riscv.h"
#include "../h/MemoryAllocator.h"
#include "../h/KSemaphore.h"

int sys_sem_open() {

    uint64 *sscratch = (uint64 *)(Riscv::r_sscratch());

    sem_t *handle = (sem_t *)(*sscratch);
    int init = (int)(*(sscratch + 1));

    *handle = (semaphore_data *)MemoryAllocator::getInstance()->mem_alloc(sizeof(semaphore_data));

    (*handle)->id = KSemaphore::addSemaphore(init);
    if ((*handle)->id >= 0) {
        (*handle)->active = true;
        return 0;
    }
    else {
        MemoryAllocator::getInstance()->mem_free(*handle);
        return -1;
    }
}

int sys_sem_close() {

    uint64 *sscratch = (uint64 *)(Riscv::r_sscratch());

    sem_t handle = (sem_t)(*sscratch);
    handle->active = false;
    return KSemaphore::removeSemaphore(handle->id);
}

int sys_sem_wait() {

    uint64 *sscratch = (uint64 *)(Riscv::r_sscratch());

    sem_t handle = (sem_t)(*sscratch);

    KSemaphore *sem = KSemaphore::getSemaphore(handle->id);
    if (!sem) return -1;
    int status = sem->wait();
    if (!sem->isActive() && !sem->haveBlocked()) {
        MemoryAllocator::getInstance()->mem_free(sem);
        KSemaphore::freeSpace(handle->id);
        MemoryAllocator::getInstance()->mem_free(handle);
    }
    return status;
}

int sys_sem_signal() {

    uint64 *sscratch = reinterpret_cast<uint64 *>(Riscv::r_sscratch());

    sem_t handle = (sem_t)(*sscratch);

    KSemaphore *sem = KSemaphore::getSemaphore(handle->id);
    if (!sem) return -1;
    if (!sem->isActive()) return -1;

    return sem->signal();
}