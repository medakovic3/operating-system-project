//
// Created by os on 6/26/22.
//

#include "../h/Scheduler.h"
#include "../h/TCB.h"
#include "../h/Riscv.h"

TCB *Scheduler::head = nullptr;
TCB *Scheduler::tail = nullptr;



void idleFunction(void *arg) {
    while (true) {
        Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
        if (Scheduler::head)
            TCB::dispatch();
    }
}

void Scheduler::put(TCB *t) {
    if (t->body != &idleFunction) {
        if (!tail)
            head = tail = t;
        else {
            tail->schedulerNext = t;
            tail = t;
        }
    }
}

TCB *Scheduler::get() {

    if (!head) {
        static TCB *idleThread = new TCB(&idleFunction, nullptr, nullptr);
        return idleThread;
    } else {
        TCB *newRunning = head;
        head = head->schedulerNext;
        if (!head)
            tail = nullptr;
        newRunning->schedulerNext = nullptr;
        return newRunning;
    }
}
