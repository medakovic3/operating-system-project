//
// Created by os on 8/21/22.
//

#include "../h/KConsole.h"
#include "../h/Riscv.h"

char sys_getc() {
    Buffer *inputBuffer = KConsole::getInstance()->getInputBuffer();
    return inputBuffer->getc();
}

void sys_putc() {

    uint64 *sscratch = (uint64 *)(Riscv::r_sscratch());

    char c = (char) (*sscratch);
    Buffer *outputBuffer = KConsole::getInstance()->getOutputBuffer();
    outputBuffer->putc(c);
}
