#include "threadtools.h"

void fibonacci(int id, int arg) {
    thread_setup(id, arg);

    for (RUNNING->i = 1; ; RUNNING->i++) {
        if (RUNNING->i <= 2)
            RUNNING->x = RUNNING->y = 1;
        else {
            int tmp = RUNNING->y;  
            RUNNING->y = RUNNING->x + RUNNING->y;
            RUNNING->x = tmp;
        }
        printf("%d %d\n", RUNNING->id, RUNNING->y);
        sleep(1);

        if (RUNNING->i == RUNNING->arg) {
            thread_exit();
        }
        else {
            thread_yield();
        }
    }
}

void collatz(int id, int arg) {
    thread_setup(id, arg);

    for(RUNNING->i = RUNNING->arg; ; ){
        RUNNING->i  = (RUNNING->i % 2 == 0)? RUNNING->i / 2 : RUNNING->i * 3 + 1;

        printf("%d %d\n", RUNNING->id, RUNNING->i);
        sleep(1);

        if (RUNNING->i == 1) {
            thread_exit();
        }
        else {
            thread_yield();
        }
    }
}

void max_subarray(int id, int arg) {
    thread_setup(id, arg);

    for (RUNNING->i = 1, RUNNING->x = RUNNING->y = 0; ; RUNNING->i++) {
        async_read(5);
        int tmp = atoi(RUNNING->buf);

        RUNNING->x = RUNNING->x + tmp > tmp? RUNNING->x + tmp : tmp;
        RUNNING->y = RUNNING->y > RUNNING->x? RUNNING->y : RUNNING->x;

        printf("%d %d\n", RUNNING->id, RUNNING->y);
        sleep(1);

        if (RUNNING->i == RUNNING->arg) {
            thread_exit();
        }
        else {
            thread_yield();
        }
    }
}
