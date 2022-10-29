//
// Created by os on 6/21/22.
//

#include "../h/syscall_c.h"

void syscall(...) {
    __asm__ volatile("ecall");
}

void *mem_alloc (size_t size) {
    size = (size % MEM_BLOCK_SIZE == 0 ? size / MEM_BLOCK_SIZE : size / MEM_BLOCK_SIZE + 1);
    syscall(SYS_mem_alloc, size);
    void *addr;
    __asm__ volatile("mv %0, a0" : "=r" (addr));
    return addr;
}

int mem_free(void *ptr) {
    syscall(SYS_mem_free, ptr);
    int status;
    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int thread_create(thread_t* handle, void (*start_routine)(void *), void *arg) {
    void *stack_space = mem_alloc(DEFAULT_STACK_SIZE);
    syscall(SYS_thread_create, handle, start_routine, arg, stack_space);
    int status;
    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int thread_exit() {
    syscall(SYS_thread_exit);
    int status;
    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

void thread_dispatch() {
    syscall(SYS_thread_dispatch);
}

int sem_open(sem_t *handle, unsigned init) {
    syscall(SYS_sem_open, handle, init);
    int status;
    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int sem_close(sem_t handle) {
    syscall(SYS_sem_close, handle);
    int status;
    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int sem_wait(sem_t id) {
    syscall(SYS_sem_wait, id);
    int status;
    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int sem_signal(sem_t id) {
    syscall(SYS_sem_signal, id);
    int status;
    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

int time_sleep(time_t time) {
    syscall(SYS_time_sleep, time);
    int status;
    __asm__ volatile("mv %0, a0" : "=r" (status));
    return status;
}

char getc() {
    syscall(SYS_getc);
    char c;
    __asm__ volatile("mv %0, a0" : "=r" (c));
    return c;
}

void putc(char c) {
    syscall(SYS_putc, c);
}




