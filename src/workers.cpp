//
// Created by os on 6/27/22.
//

#include "../lib/hw.h"
#include "../h/print.h"
#include "../h/TCB.h"
#include "../h/syscall_c.h"

void workerBodyA(void *arg)
{
    for (uint64 i = 0; i < 10; i++)
    {
        printString("A: i=");
        printInteger(i);
        printString("\n");

        //time_sleep(1);
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield();
        }
    }
}

void workerBodyB(void *arg)
{
    for (uint64 i = 0; i < 16; i++)
    {
        printString("B: i=");
        printInteger(i);
        printString("\n");

        //time_sleep(2);
        for (uint64 j = 0; j < 10000; j++)
        {
            for (uint64 k = 0; k < 30000; k++)
            {
                // busy wait
            }
//            TCB::yield();
        }
    }
}

static uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 10 == 0) { thread_dispatch(); }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyC(void *arg)
{
    uint8 i = 0;
    for (; i < 3; i++)
    {
        printString("C: i=");
        printInteger(i);
        printString("\n");
    }
    //time_sleep(3);
    printString("C: yield\n");
    __asm__ ("li t1, 7");
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printString("C: t1=");
    printInteger(t1);
    printString("\n");

    uint64 result = fibonacci(12);
    printString("C: fibonaci=");
    printInteger(result);
    printString("\n");

    for (; i < 6; i++)
    {
        printString("C: i=");
        printInteger(i);
        printString("\n");
    }
    thread_dispatch();
}


void workerBodyD(void *arg)
{
    uint8 i = 10;
    for (; i < 13; i++)
    {
        printString("D: i=");
        printInteger(i);
        printString("\n");
    }
    //time_sleep(4);
    printString("D: yield\n");
    __asm__ ("li t1, 5");
    thread_dispatch();

    uint64 result = fibonacci(16);
    printString("D: fibonaci=");
    printInteger(result);
    printString("\n");

    for (; i < 16; i++)
    {
        printString("D: i=");
        printInteger(i);
        printString("\n");
    }
    thread_dispatch();
}

int x = 10;

void semTest1(void *arg) {
    for(int i = 0; i < 8; i++) {
        sem_t id = (sem_t) arg;
        sem_wait(id);
        x++;
        printString("x = ");
        printInteger(x);
        printString("\n");
        time_sleep(2);
        sem_signal(id);
    }
}

void semTest2(void *arg) {
    for(int i = 0; i < 8; i++) {
        sem_t id = (sem_t) arg;
        sem_wait(id);
        x--;
        printString("x = ");
        printInteger(x);
        printString("\n");
        time_sleep(2);
        sem_signal(id);
    }
}

const int N = 5;
int buffer[] = {0, 0, 0, 0, 0};
int head = 0, tail = 0, cnt = 0;

void put(int i);
void printBuffer();

void producer(void *arg) {
    sem_t mutex = ((semaphores*)arg)->mutex;
    sem_t spaceAvailable = ((semaphores*)arg)->spaceAvailable;
    sem_t itemAvailable = ((semaphores*)arg)->itemAvailable;
    int i = 0;
    while (true) {
        cnt = (cnt + 1) % 40;
        i = (3 * i + (cnt % 11)) % (N + i) + 1;
        sem_wait(spaceAvailable);
        sem_wait(mutex);
        put(i);
        printBuffer();
        sem_signal(mutex);
        sem_signal(itemAvailable);
        time_sleep(cnt < 21 ? 1 : 3);
    }
}

void get();

void consumer(void *arg) {
    sem_t mutex = ((semaphores*)arg)->mutex;
    sem_t spaceAvailable = ((semaphores*)arg)->spaceAvailable;
    sem_t itemAvailable = ((semaphores*)arg)->itemAvailable;
    while (true) {
        sem_wait(itemAvailable);
        sem_wait(mutex);
        get();
        printBuffer();
        sem_signal(mutex);
        sem_signal(spaceAvailable);
        time_sleep(cnt < 21 ? 3 : 1);
    }
}

void put (int i) {
    buffer[tail] = i;
    tail = (tail + 1) % N;
}

void get() {
    buffer[head] = 0;
    head = (head + 1) % N;
}

void printBuffer(){
    for (int i = 0; i < N; i++) {
        buffer[i] != 0 ? printInteger(buffer[i]) : printString("_");
        i != N - 1 ? printString(" ") : printString("\n");
    }
}

sem_t handle;

void semCloseTest(void *arg) {
    sem_wait(handle);
    printString("Prosla sam!\n");
}
