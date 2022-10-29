//
// Created by os on 8/20/22.
//

#include "../h/Buffer.h"

void Buffer::putc(char c) {
    spaceAvailable.wait();
    buffer[tail] = c;
    tail = (tail + 1) % BUFFER_SIZE;
    count++;
    itemAvailable.signal();
}

char Buffer::getc() {
    itemAvailable.wait();
    char c = buffer[head];
    head = (head + 1) % BUFFER_SIZE;
    count--;
    spaceAvailable.signal();
    return c;
}
