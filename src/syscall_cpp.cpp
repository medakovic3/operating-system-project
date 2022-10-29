//
// Created by os on 5/25/22.
//

#include "../h/syscall_cpp.h"
#include "../h/syscall_c.h"


void* operator new(size_t size) {
    return mem_alloc(size);
}

void *operator new[](size_t size) {
    return mem_alloc(size);
}

void operator delete(void* ptr) {
    mem_free(ptr);
}

void operator delete[](void *ptr)  {
    mem_free(ptr);
}


void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

int Thread::start() {
    return thread_create(&myHandle, &runWrapper, (void*)(this));
}

void Thread::runWrapper(void *arg) {
    Thread *t = (Thread*) arg;
    t->run();
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}
