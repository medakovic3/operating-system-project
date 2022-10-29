//
// Created by os on 8/19/22.
//

#include "../h/KConsole.h"
#include "../h/print.h"


KConsole *KConsole::instance = nullptr;

void KConsole::print(void *arg) {
    static Buffer *outputBuffer = KConsole::getInstance()->outputBuffer;
    static char *status = (char *) CONSOLE_STATUS;
    static char *outputRegister = (char *) CONSOLE_TX_DATA;

    while (true) {
        if (*status & CONSOLE_TX_STATUS_BIT) {
            char c = outputBuffer->getc();
            *outputRegister = c;
        }
        else {
            printString("Not ready to receive next character\n");
            TCB *old = TCB::running;
            TCB::running = Scheduler::get();
            TCB::contextSwitch(&old->context, &TCB::running->context);
        }
    }

}

void KConsole::scan(void *arg) {
    static Buffer *inputBuffer = KConsole::getInstance()->inputBuffer;
    static char *status = (char *) CONSOLE_STATUS;
    static char *inputRegister = (char *) CONSOLE_RX_DATA;

    while (true) {
        if (*status & CONSOLE_RX_STATUS_BIT) {
            char c = *inputRegister;
            inputBuffer->putc(c);
        }
        else {
            TCB *old = TCB::running;
            TCB::running = Scheduler::get();
            TCB::contextSwitch(&old->context, &TCB::running->context);
        }
    }


}
