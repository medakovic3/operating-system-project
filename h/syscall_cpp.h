//
// Created by os on 5/25/22.
//

#ifndef PROJECT_BASE_SYSCALL_CPP_H
#define PROJECT_BASE_SYSCALL_CPP_H

#include "../lib/mem.h"
#include "MemoryAllocator.h"
#include "handles.h"


void* operator new (size_t);
void* operator new[] (size_t);
void operator delete (void*);
void operator delete[] (void*);

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();

    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    static void runWrapper(void *arg);
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};


#endif //PROJECT_BASE_SYSCALL_CPP_H
