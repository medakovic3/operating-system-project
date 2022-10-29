//
// Created by os on 8/11/22.
//

#include "../h/Sleep.h"


TCB *Sleep::head = nullptr;

int Sleep::sleep(time_t sleepTime) {
    if (sleepTime <= 0) return -1;

    if (!head) {
        head = TCB::running;
        TCB::running->sleepNext = nullptr;
        TCB::running->sleepTime = sleepTime;
    }
    else {
        int difference = sleepTime - head->sleepTime;

        if (difference <= 0) {
            TCB::running->sleepNext = head;
            head->sleepTime = -difference;
            head = TCB::running;
            head->sleepTime = sleepTime;
        }
        else {
            TCB *curr = head->sleepNext;
            TCB *prev = head;

            for (; curr; prev = curr, curr = curr->sleepNext) {

                if (difference < (int) curr->sleepTime) break;
                else difference -= curr->sleepTime;
            }

            prev->sleepNext = TCB::running;
            TCB::running->sleepNext = curr;
            if (curr) curr->sleepTime -= difference;
            TCB::running->sleepTime = difference;
        }
    }

    TCB *old = TCB::running;
    TCB::running = Scheduler::get();
    TCB::contextSwitch(&old->context, &TCB::running->context);
    return 0;
}

void Sleep::wakeUp() {

    for (; head; head = head->sleepNext) {
        if (head->sleepTime != 0) break;
        Scheduler::put(head);
    }
}

void Sleep::update() {
    if (!head) return;

    if (--(head->sleepTime) == 0) wakeUp();
}






