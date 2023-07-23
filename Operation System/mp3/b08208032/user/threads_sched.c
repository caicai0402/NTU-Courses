#include "kernel/types.h"
#include "user/user.h"
#include "user/list.h"
#include "user/threads.h"
#include "user/threads_sched.h"

#define NULL 0
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* default scheduling algorithm */
struct threads_sched_result schedule_default(struct threads_sched_args args)
{
    struct thread *thread_with_smallest_id = NULL;
    struct thread *th = NULL;
    list_for_each_entry(th, args.run_queue, thread_list) {
        if (thread_with_smallest_id == NULL || th->ID < thread_with_smallest_id->ID) {
            thread_with_smallest_id = th;
        }
    }

    struct threads_sched_result r;
    if (thread_with_smallest_id != NULL) {
        r.scheduled_thread_list_member = &thread_with_smallest_id->thread_list;
        r.allocated_time = thread_with_smallest_id->remaining_time;
    } else {
        r.scheduled_thread_list_member = args.run_queue;
        r.allocated_time = 1;
    }

    return r;
}

/* Earliest-Deadline-First scheduling */
struct threads_sched_result schedule_edf(struct threads_sched_args args)
{
    struct threads_sched_result r;

    if(args.run_queue->next != args.run_queue){ // Check whether run queue is empty or not.
        struct thread *missed_thread = NULL;
        struct thread *thread_with_earliest_deadline_in_run_queue = NULL;
        struct thread *th1 = NULL;
        list_for_each_entry(th1, args.run_queue, thread_list) {
            if (th1->current_deadline < args.current_time && 
                (missed_thread == NULL || 
                    th1->ID <= missed_thread->ID)
                ) {
                    missed_thread = th1;
            }

            if (thread_with_earliest_deadline_in_run_queue == NULL || 
                th1->current_deadline < thread_with_earliest_deadline_in_run_queue->current_deadline || 
                (th1->current_deadline == thread_with_earliest_deadline_in_run_queue->current_deadline && 
                    th1->ID < thread_with_earliest_deadline_in_run_queue->ID)
                ) {

                thread_with_earliest_deadline_in_run_queue = th1;
            }
        }

        if(missed_thread != NULL){
            r.scheduled_thread_list_member = &missed_thread->thread_list;
            r.allocated_time = 0;
            return r;
        }

        struct thread *thread_with_earliest_deadline_in_release_queue = NULL;
        struct release_queue_entry *th2 = NULL;
        list_for_each_entry(th2, args.release_queue, thread_list) {
            if ((th2->thrd->current_deadline + th2->thrd->period < thread_with_earliest_deadline_in_run_queue->current_deadline ||
                    (th2->thrd->current_deadline + th2->thrd->period == thread_with_earliest_deadline_in_run_queue->current_deadline && 
                        th2->thrd->ID < thread_with_earliest_deadline_in_release_queue->ID)) &&
                (thread_with_earliest_deadline_in_release_queue == NULL ||
                    th2->thrd->current_deadline + th2->thrd->period < thread_with_earliest_deadline_in_release_queue->current_deadline + thread_with_earliest_deadline_in_release_queue->period)
                ) {

                thread_with_earliest_deadline_in_release_queue = th2->thrd;
            }
        }

        r.scheduled_thread_list_member = &thread_with_earliest_deadline_in_run_queue->thread_list;
        if (thread_with_earliest_deadline_in_release_queue == NULL) {
            r.allocated_time = MIN(thread_with_earliest_deadline_in_run_queue->remaining_time, thread_with_earliest_deadline_in_run_queue->current_deadline - args.current_time);
        }
        else{
            r.allocated_time = MIN(thread_with_earliest_deadline_in_run_queue->remaining_time, thread_with_earliest_deadline_in_release_queue->current_deadline - args.current_time);
        }
        
    }
    else{
        struct release_queue_entry *entry_with_earliest_release_time = NULL;
        struct release_queue_entry *th = NULL;
        list_for_each_entry(th, args.release_queue, thread_list) {
            if (entry_with_earliest_release_time == NULL || 
                th->release_time < entry_with_earliest_release_time->release_time
                ) {

                entry_with_earliest_release_time = th;
            }
        }
        
        r.scheduled_thread_list_member = args.run_queue;
        r.allocated_time = entry_with_earliest_release_time == NULL? 1 : entry_with_earliest_release_time->release_time - args.current_time;
    }
    
    return r;
}

/* Rate-Monotonic Scheduling */
struct threads_sched_result schedule_rm(struct threads_sched_args args)
{
    struct threads_sched_result r;

    if(args.run_queue->next != args.run_queue){ // Check whether run queue is empty or not.
        struct thread *missed_thread = NULL;
        struct thread *thread_with_shortest_period_in_run_queue = NULL;
        struct thread *th1 = NULL;

        list_for_each_entry(th1, args.run_queue, thread_list) {
            if (th1->current_deadline <= args.current_time && 
                (missed_thread == NULL || 
                    th1->ID < missed_thread->ID)
                ) {
                    missed_thread = th1;
            }
            
            if (thread_with_shortest_period_in_run_queue == NULL || 
                th1->period < thread_with_shortest_period_in_run_queue->period || 
                (th1->period == thread_with_shortest_period_in_run_queue->period &&
                    th1->ID < thread_with_shortest_period_in_run_queue->ID)
                ) {

                thread_with_shortest_period_in_run_queue = th1;
            }
        }

        if(missed_thread != NULL){
            r.scheduled_thread_list_member = &missed_thread->thread_list;
            r.allocated_time = 0;
            return r;
        }

        struct thread *thread_with_shortest_period_in_release_queue = NULL;
        struct release_queue_entry *th2 = NULL;

        list_for_each_entry(th2, args.release_queue, thread_list) {
            if ((th2->thrd->period < thread_with_shortest_period_in_run_queue->period ||
                    (th2->thrd->period == thread_with_shortest_period_in_run_queue->period &&
                        th2->thrd->ID < thread_with_shortest_period_in_run_queue->ID)) &&
                (thread_with_shortest_period_in_release_queue == NULL ||
                    th2->thrd->current_deadline < thread_with_shortest_period_in_release_queue->current_deadline)
                ) {

                thread_with_shortest_period_in_release_queue = th2->thrd;
            }
        }

        r.scheduled_thread_list_member = &thread_with_shortest_period_in_run_queue->thread_list;
        if (thread_with_shortest_period_in_release_queue == NULL || 
            MIN(args.current_time + thread_with_shortest_period_in_run_queue->remaining_time, thread_with_shortest_period_in_run_queue->current_deadline) <= thread_with_shortest_period_in_release_queue->current_deadline
            ) {           
            r.allocated_time = MIN(thread_with_shortest_period_in_run_queue->remaining_time, thread_with_shortest_period_in_run_queue->current_deadline - args.current_time);
        }
        else{
            r.allocated_time = thread_with_shortest_period_in_release_queue->current_deadline - args.current_time;
        }
    }
    else{
        struct release_queue_entry *entry_with_earliest_release_time = NULL;
        struct release_queue_entry *th = NULL;
        list_for_each_entry(th, args.release_queue, thread_list) {
            if (entry_with_earliest_release_time == NULL || 
                th->release_time < entry_with_earliest_release_time->release_time
                ) {

                entry_with_earliest_release_time = th;
            }
        }
        
        r.scheduled_thread_list_member = args.run_queue;
        r.allocated_time = entry_with_earliest_release_time == NULL? 1 : entry_with_earliest_release_time->release_time - args.current_time;
    }

    return r;
}
