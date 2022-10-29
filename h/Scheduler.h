//
// Created by os on 6/26/22.
//

#ifndef PROJECT_BASE_1_1_SCHEDULER_H
#define PROJECT_BASE_1_1_SCHEDULER_H

class TCB;

    class Scheduler {

    public:

        static TCB *head;
        static TCB *tail;

        static void put(TCB *t);

        static TCB *get();

    };


#endif //PROJECT_BASE_1_1_SCHEDULER_H
