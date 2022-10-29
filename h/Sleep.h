//
// Created by os on 8/11/22.
//

#ifndef PROJECT_BASE_1_1_SLEEP_H
#define PROJECT_BASE_1_1_SLEEP_H


#include "TCB.h"

class Sleep {
public:
    static int sleep(time_t sleepTime);
    static void wakeUp();
    static void update();

private:
    static TCB *head;
};


#endif //PROJECT_BASE_1_1_SLEEP_H
