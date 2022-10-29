//
// Created by os on 6/21/22.
//

#ifndef PROJECT_BASE_SYSCALL_C_H
#define PROJECT_BASE_SYSCALL_C_H

#include "../lib/hw.h"
#include "handles.h"


//System call numbers
enum SystemCalls {  SYS_mem_alloc = 0x01,
                    SYS_mem_free = 0x02,
                    SYS_thread_create = 0x11,
                    SYS_thread_exit = 0x12,
                    SYS_thread_dispatch = 0x13,
                    SYS_sem_open = 0x21,
                    SYS_sem_close = 0x22,
                    SYS_sem_wait = 0x23,
                    SYS_sem_signal = 0x24,
                    SYS_time_sleep = 0x31,
                    SYS_getc = 0x41,
                    SYS_putc = 0x42
                 };

void *mem_alloc(size_t size);
int mem_free(void *ptr);
int thread_create (thread_t* handle, void (*start_routine)(void *) , void *arg);
int thread_exit ();
void thread_dispatch();
int sem_open(sem_t *handle, unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int time_sleep(time_t time);
char getc();
void putc(char c);

#endif //PROJECT_BASE_SYSCALL_C_H
