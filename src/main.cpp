//
// Created by os on 5/25/22.
//

#ifndef MAIN
#define MAIN

#include "../h/Riscv.h"
#include "../h/print.h"
#include "../h/syscall_c.h"
#include "../h/KConsole.h"

void userMain();

sem_t mainSemHandle;

void main() {
    Riscv::w_stvec((uint64) &Riscv::trap);

    TCB *mainThread = new TCB(nullptr, nullptr, nullptr);
    TCB::running = mainThread;
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    thread_t handleUserMain;
    thread_create(&handleUserMain, (void (*)(void *))(&userMain), nullptr);

    sem_open(&mainSemHandle, 0);

    sem_wait(mainSemHandle);
}

#endif
