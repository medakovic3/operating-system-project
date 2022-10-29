//
// Created by os on 6/1/22.
//

#include "../h/Riscv.h"
#include "../h/print.h"
#include "../h/syscall_c.h"
#include "../h/syscall_mem.h"
#include "../h/syscall_thread.h"
#include "../h/syscall_sem.h"
#include "../h/syscall_console.h"
#include "../h/TCB.h"
#include "../h/Sleep.h"
#include "../h/KConsole.h"


void Riscv::popSppSpie() {
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile ("csrw sepc, ra");
    __asm__ volatile ("sret");
}

void Riscv::pop_a0() {
    uint64 scause = r_scause();
    if (!(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL))
        __asm__ volatile ("ld a0, 80(s0)");
}

void Riscv::handleTrap() {

    uint64 scause = r_scause();
    uint64 sepc = r_sepc();
    uint64 sstatus = r_sstatus();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)

        sepc += 4;

        int code = r_a0();

        switch (code) {
        case SYS_mem_alloc: sys_mem_alloc(); break;
        case SYS_mem_free: sys_mem_free(); break;
        case SYS_thread_create: sys_thread_create(); break;
        case SYS_thread_exit: sys_thread_exit(); break;
        case SYS_thread_dispatch: break;
        case SYS_sem_open: sys_sem_open(); break;
        case SYS_sem_close: sys_sem_close(); break;
        case SYS_sem_wait: sys_sem_wait(); break;
        case SYS_sem_signal: sys_sem_signal(); break;
        case SYS_time_sleep: sys_time_sleep(); break;
        case SYS_getc: sys_getc(); break;
        case SYS_putc: sys_putc(); break;
        }

        TCB::timeSliceCounter = 0;

        uint64 syscallReturnValue = r_a0();
        if (code != SYS_sem_wait && code != SYS_sem_signal && code != SYS_time_sleep && code != SYS_putc) TCB::dispatch();
        w_a0(syscallReturnValue);

        mc_sip(SIP_SSIP);

    } else if (scause == 0x8000000000000001UL) {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)

        Sleep::update();
        TCB::timeSliceCounter++;

        if(TCB::timeSliceCounter >= TCB::running->timeSlice) {
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }

        mc_sip(SIP_SSIP);

    } else if (scause == 0x8000000000000009UL) {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        if (plic_claim() == CONSOLE_IRQ) {

            static volatile const char *status = (char *) CONSOLE_STATUS;
            static KConsole *console = KConsole::getInstance();
            TCB *printThread = console->getPrintThread();
            TCB *scanThread = console->getScanThread();

            if (*status & CONSOLE_TX_STATUS_BIT && !printThread->isBlocked()) {
                TCB *old = TCB::running;
                Scheduler::put(old);
                TCB::running = printThread;
                TCB::contextSwitch(&old->context, &TCB::running->context);
            }

            if (*status & CONSOLE_RX_STATUS_BIT && !scanThread->isBlocked()) {
                TCB *old = TCB::running;
                Scheduler::put(old);
                TCB::running = scanThread;
                TCB::contextSwitch(&old->context, &TCB::running->context);
            }
            plic_complete(CONSOLE_IRQ);
        }

    } else {

        switch (scause) {
            case 0x0000000000000002UL:
                printString("Illegal instruction!\n");
                break;
            case 0x0000000000000005UL:
                printString("Read access violation!\n");
                break;
            case 0x0000000000000007UL:
                printString("Write access violation!\n");
                break;
            default:
                printString("Unexpected interrupt!\n");
                break;
        }
    }
    w_sstatus(sstatus);
    w_sepc(sepc);
    w_scause(scause);
}
