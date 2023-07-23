#include "kernel/types.h"
#include "user/setjmp.h"
#include "user/threads.h"
#include "user/user.h"
#define NULL 0


static struct thread* current_thread = NULL;
static int id = 1;
static jmp_buf env_st;
static jmp_buf env_tmp;

struct thread *thread_create(void (*f)(void *), void *arg){
    struct thread *t = (struct thread*) malloc(sizeof(struct thread));
    unsigned long new_stack_p;
    unsigned long new_stack;
    new_stack = (unsigned long) malloc(sizeof(unsigned long)*0x100);
    new_stack_p = new_stack + 0x100*8-0x2*8;
    t->fp = f;
    t->arg = arg;
    t->ID  = id;
    t->buf_set = 0;
    t->stack = (void*) new_stack;
    t->stack_p = (void*) new_stack_p;
    id++;

    // part 2
    t->sig_handler[0] = NULL_FUNC;
    t->sig_handler[1] = NULL_FUNC;
    t->signo = -1;
    t->handler_buf_set = 0;
    return t;
}
void thread_add_runqueue(struct thread *t){
    if(current_thread == NULL){
        t->next = t->previous = t;
        current_thread = t;
        return ;
    }
    else{
        t->sig_handler[0] = current_thread->sig_handler[0];
        t->sig_handler[1] = current_thread->sig_handler[1];
        current_thread->previous->next = t;
        t->previous = current_thread->previous;
        current_thread->previous = t;
        t->next = current_thread;
        return ;
    }
}
void thread_yield(void){
    if(current_thread->signo == -1){
        if(setjmp(current_thread->env) == 0){
            current_thread->buf_set = 1;
            schedule();
            dispatch();
        }
    }
    else{
        if(setjmp(current_thread->handler_env) == 0){
            current_thread->handler_buf_set = 1;
            schedule();
            dispatch();
        }
    }
    return ;
}
void dispatch(void){
    if(current_thread->signo != -1){
        if(current_thread->sig_handler[current_thread->signo] == NULL_FUNC)
            thread_exit();
        else if(current_thread->handler_buf_set == 0){
            if(setjmp(env_tmp) == 0){
                env_tmp->sp = current_thread->buf_set? current_thread->env->sp : (unsigned long)current_thread->stack_p;
                longjmp(env_tmp, 1);
            }
            else{
                current_thread->sig_handler[current_thread->signo](current_thread->signo);
                current_thread->signo = -1;
            }
        }
        else
            longjmp(current_thread->handler_env, 1);
    }
    if(current_thread->signo == -1){
        if(current_thread->buf_set == 0){
            if(setjmp(env_tmp) == 0){
                env_tmp->sp = (unsigned long)current_thread->stack_p;
                longjmp(env_tmp, 1);
            }
            else{
                current_thread->fp(current_thread->arg);
                thread_exit();
            }
        }
        else
            longjmp(current_thread->env, 1);
    }
    return ;
}
void schedule(void){
    current_thread = current_thread->next;
    return ;
}
void thread_del(struct thread *t){
    if(t != NULL){
        t->previous->next = t->next;
        t->next->previous = t->previous;
        free(t->stack);
        free(t);
    }
    return ;
}
void thread_exit(void){
    if(current_thread->next != current_thread){
        struct thread *tmp = current_thread;
        schedule();
        thread_del(tmp);
        dispatch();
    }
    else{
        thread_del(current_thread);
        longjmp(env_st, 1);
    }
    return ;
}
void thread_start_threading(void){
    if(setjmp(env_st) == 0)
        dispatch();
    return ;
}
// part 2
void thread_register_handler(int signo, void (*handler)(int)){
    current_thread->sig_handler[signo] = handler;
    return ;
}
void thread_kill(struct thread *t, int signo){
    t->signo = signo;
    return ;
}