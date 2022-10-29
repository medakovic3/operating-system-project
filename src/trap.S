# 1 "src/trap.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/riscv64-linux-gnu/include/stdc-predef.h" 1 3
# 32 "<command-line>" 2
# 1 "src/trap.S"
.extern _ZN5Riscv10handleTrapEv
.extern _ZN3TCB19changeToSystemStackEv
.extern _ZN3TCB17changeToUserStackEv
.extern _ZN5Riscv7push_a0Ev
.extern _ZN5Riscv6pop_a0Ev
.align 4
.global _ZN5Riscv4trapEv
.type _ZN5Riscv4trapEv, @function
_ZN5Riscv4trapEv:

    #push the a1-a7 registers because functions might change them
    addi sp, sp, -64
   .irp index, 11,12,13,14,15,16,17
    sd x\index, (\index - 11) * 8(sp)
   .endr

    # save the pointer on a1-a7 registers in sscratch register
    csrw sscratch, sp

    # change to system stack, push the ra registar because function will change it
    addi sp, sp, -16
    sd ra, 0(sp)
    call _ZN3TCB19changeToSystemStackEv

    addi sp, sp, -256
    # push all (but a1-a7) registers to stack
   .irp index, 0,1,2,3,4,5,6,7,8,9,10,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
   .endr

    call _ZN5Riscv10handleTrapEv

    #pop a0 from stack if cause is not an enviroment call
    call _ZN5Riscv6pop_a0Ev

    # pop all (but a0-a7) registers from stack
   .irp index, 0,1,2,3,4,5,6,7,8,9,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
   .endr
    addi sp, sp, 256

    #change to user stack, pop the ra register
    call _ZN3TCB17changeToUserStackEv
    ld ra, 0(sp)
    addi sp, sp, 16

    #pop the a1-a7 registers
   .irp index, 11,12,13,14,15,16,17
    ld x\index, (\index - 11) * 8(sp)
   .endr
    addi sp, sp, 64

    sret
