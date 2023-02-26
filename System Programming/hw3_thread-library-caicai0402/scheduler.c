#include "threadtools.h"

bool isready(struct tcb *thread){
    struct pollfd fdarr[1] = {{thread->fd, POLLIN}};
	return poll(fdarr, 1, 0) == 1;
}

void waiting2ready(){
    int new_wq_size = 0;
    for(int i = 0; i < wq_size; i++){
        if(isready(waiting_queue[i]))
            ready_queue[rq_size ++] = waiting_queue[i];
        else
            waiting_queue[new_wq_size ++] = waiting_queue[i];
    }
    wq_size = new_wq_size;
    return ;
}

/*
 * Print out the signal you received.
 * If SIGALRM is received, reset the alarm here.
 * This function should not return. Instead, call longjmp(sched_buf, 1).
 */
void sighandler(int signo) {
    sigprocmask(SIG_SETMASK, &base_mask, NULL);
    if(signo == SIGTSTP){
        printf("caught SIGTSTP\n");
    }
    else if(signo == SIGALRM){
        printf("caught SIGALRM\n");
        alarm(timeslice);
    }
    longjmp(sched_buf, 1);
}

/*
 * Prior to calling this function, both SIGTSTP and SIGALRM should be blocked.
 */
void scheduler() {
    switch(setjmp(sched_buf)){
        case 0:
            rq_current = 0;
            break;
            
        case 1: // thread yield
            waiting2ready();
            rq_current = rq_current == rq_size - 1? 0 : rq_current + 1;
            break;

        case 2: // async read
            waiting2ready();
            waiting_queue[wq_size ++] = RUNNING;
            RUNNING = ready_queue[-- rq_size];
            rq_current = rq_current == rq_size? 0 : rq_current;
            break;

        case 3: // thread exit
            waiting2ready();
            unlink(RUNNING->fifoname);
            free(RUNNING);
            RUNNING = ready_queue[-- rq_size];
            rq_current = rq_current == rq_size? 0 : rq_current;
            break;
    }
    if(rq_size == 0 && wq_size == 0)
        return ;
    else{
        while(rq_size == 0)
            waiting2ready();
        longjmp(RUNNING->env, 1);
    }
}
