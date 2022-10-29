//
// Created by os on 6/29/22.
//

#ifndef PROJECT_BASE_1_1_HANDLES_H
#define PROJECT_BASE_1_1_HANDLES_H

class Thread;

using Body = void (*)(void*);
using RunBody = void (*)();

struct thread_data {
    int id;
    Body body;
    void *arg;
};

typedef thread_data *thread_t;

struct semaphore_data {
    int id;
    bool active;
};

typedef semaphore_data *sem_t;

struct semaphores {
    sem_t mutex, spaceAvailable, itemAvailable;
    semaphores(sem_t m, sem_t sa, sem_t ia) {
        mutex = m;
        spaceAvailable = sa;
        itemAvailable = ia;
    }
};

#endif //PROJECT_BASE_1_1_HANDLES_H







