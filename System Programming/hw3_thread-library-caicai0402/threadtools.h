#ifndef THREADTOOL
#define THREADTOOL
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <setjmp.h>
#include <sys/signal.h>
#include <sys/stat.h>

#define THREAD_MAX 16  // maximum number of threads created
#define BUF_SIZE 512
struct tcb {
    int id;  // the thread id
    int arg;  // argument to the function
    int fd;  // file descriptor for the thread
    jmp_buf env;  // where the scheduler should jump to
    char buf[BUF_SIZE];  // buffer for the thread
    char fifoname[BUF_SIZE];
    int i, x, y;  // declare the variables you wish to keep between switches
};

extern int timeslice;
extern jmp_buf sched_buf;
extern struct tcb *ready_queue[THREAD_MAX], *waiting_queue[THREAD_MAX];
/*
 * rq_size: size of the ready queue
 * rq_current: current thread in the ready queue
 * wq_size: size of the waiting queue
 */
extern int rq_size, rq_current, wq_size;
/*
* base_mask: blocks both SIGTSTP and SIGALRM
* tstp_mask: blocks only SIGTSTP
* alrm_mask: blocks only SIGALRM
*/
extern sigset_t base_mask, tstp_mask, alrm_mask;
/*
 * Use this to access the running thread.
 */
#define RUNNING (ready_queue[rq_current])

void sighandler(int signo);
void scheduler();

#define thread_create(func, id, arg) {\
    func(id, arg);\
}

#define thread_setup(id, arg) {\
    if(ready_queue[0] == NULL){ rq_size = wq_size = 0; }\
    struct tcb *ptr = ready_queue[rq_size++] = malloc(sizeof(struct tcb));\
    ptr->id = id;\
    ptr->arg = arg;\
    sprintf(ptr->fifoname, "%d_%s", ptr->id, __func__);\
    mkfifo(ptr->fifoname, 0777);\
    ptr->fd = open(ptr->fifoname, O_RDONLY | O_NONBLOCK);\
    if(setjmp(ptr->env) == 0)\
        return ;\
}

#define thread_exit() {\
    longjmp(sched_buf, 3);\
}

#define thread_yield() {\
    if(setjmp(RUNNING->env) == 0){\
        sigprocmask(SIG_UNBLOCK, &tstp_mask, NULL);\
        sigprocmask(SIG_UNBLOCK, &alrm_mask, NULL);\
        sigprocmask(SIG_SETMASK, &base_mask, NULL);\
    }\
}

#define async_read(count) {\
    if(setjmp(RUNNING->env) == 0)\
        longjmp(sched_buf, 2);\
    read(RUNNING->fd, RUNNING->buf, count);\
}

#endif // THREADTOOL
