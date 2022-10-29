//
// Created by os on 6/2/22.
//

#include "../h/print.h"
#include "../h/TCB.h"
#include "../h/syscall_c.h"
#include "../h/Riscv.h"

void workerBodyA(void *arg);
void workerBodyB(void *arg);
void workerBodyC(void *arg);
void workerBodyD(void *arg);
void semTest1(void *arg);
void semTest2(void *arg);
void producer(void *arg);
void consumer(void *arg);


extern const int N;
extern sem_t handle;

void semCloseTest(void *arg);

extern sem_t mainSemHandle;

void userMain() {

    //myPrintString("Hello World!\n");
    while (true) putc(getc());

    /*thread_t handleA;
    thread_create(&handleA, &workerBodyA, nullptr);
    TCB *workerThreadA = TCB::getThread(handleA->id);

    thread_t handleB;
    thread_create(&handleB, &workerBodyB, nullptr);
    TCB *workerThreadB = TCB::getThread(handleB->id);

    thread_t handleC;
    thread_create(&handleC, &workerBodyC, nullptr);
    TCB *workerThreadC = TCB::getThread(handleC->id);

    thread_t handleD;
    thread_create(&handleD, &workerBodyD, nullptr);
    TCB *workerThreadD = TCB::getThread(handleD->id);

    while (!(workerThreadA->isFinished() && workerThreadB->isFinished() && workerThreadC->isFinished() &&
             workerThreadD->isFinished()))
        thread_dispatch();*/

    /*sem_t handleSem;
    sem_open(&handleSem, 1);

    thread_t handle1;
    thread_create(&handle1, &semTest1, handleSem);
    TCB *semThread1 = TCB::getThread(handle1->id);

    thread_t handle2;
    thread_create(&handle2, &semTest2, handleSem);
    TCB *semThread2 = TCB::getThread(handle2->id);

    while (!(workerThreadA->isFinished() && workerThreadB->isFinished() && workerThreadC->isFinished() &&
            workerThreadD->isFinished() && semThread1->isFinished() && semThread2->isFinished()))
        thread_dispatch();

    sem_close(handleSem);*/

    /*sem_t handleMutex, handleSpaceAvailable, handleItemAvailable;
    sem_open(&handleMutex, 1);
    sem_open(&handleSpaceAvailable, 5);
    sem_open(&handleItemAvailable, 0);

    semaphores *s = new semaphores(handleMutex, handleSpaceAvailable, handleItemAvailable);

    thread_t handleP;
    thread_create(&handleP, &producer, s);
    TCB *producerT = TCB::getThread(handleP->id);

    thread_t handleC;
    thread_create(&handleC, &consumer, s);
    TCB *consumerT = TCB::getThread(handleC->id);

    while (!(producerT->isFinished() && consumerT->isFinished()))
        thread_dispatch();

    sem_close(handleMutex);
    sem_close(handleItemAvailable);
    sem_close(handleSpaceAvailable);*/

    /*sem_open(&handle, 0);

    thread_t handleA;
    thread_create(&handleA, &semCloseTest, nullptr);
    TCB *workerThreadA = TCB::getThread(handleA->id);

    thread_t handleB;
    thread_create(&handleB, &semCloseTest, nullptr);
    TCB *workerThreadB = TCB::getThread(handleB->id);

    thread_t handleC;
    thread_create(&handleC, &semCloseTest, nullptr);
    TCB *workerThreadC = TCB::getThread(handleC->id);

    thread_t handleD;
    thread_create(&handleD, &semCloseTest, nullptr);
    TCB *workerThreadD = TCB::getThread(handleD->id);

    sem_close(handle);

    while (!(workerThreadA->isFinished() && workerThreadB->isFinished() && workerThreadC->isFinished() &&
             workerThreadD->isFinished()))
        thread_dispatch();*/

    sem_signal(mainSemHandle);
}