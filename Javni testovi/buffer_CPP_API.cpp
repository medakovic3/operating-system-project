#include "buffer_CPP_API.hpp"
#include "../h/std.h"


BufferCPP::BufferCPP(int _cap) : cap(_cap), head(0), tail(0) {
    buffer = (int *)mem_alloc(sizeof(int) * cap);
    itemAvailable = new Semaphore(0);
    spaceAvailable = new Semaphore(cap);
    mutexHead = new Semaphore(1);
    mutexTail = new Semaphore(1);
}

BufferCPP::~BufferCPP() {
    putc('\n');
    printf("Buffer deleted!\n");
    while (head != tail) {
        printf("%c ", buffer[head]);
        head = (head + 1) % cap;
    }
    putc('!');
    putc('\n');

    mem_free(buffer);
    delete itemAvailable;
    delete spaceAvailable;
    delete mutexTail;
    delete mutexHead;

}

void BufferCPP::put(int val) {
    spaceAvailable->wait();

    mutexTail->wait();
    buffer[tail] = val;
    tail = (tail + 1) % cap;
    mutexTail->signal();

    itemAvailable->signal();

}

int BufferCPP::get() {
    itemAvailable->wait();

    mutexHead->wait();

    int ret = buffer[head];
    head = (head + 1) % cap;
    mutexHead->signal();

    spaceAvailable->signal();

    return ret;
}
