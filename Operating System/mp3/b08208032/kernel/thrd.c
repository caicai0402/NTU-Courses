#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

// for mp3
uint64
sys_thrdstop(void)
{
  int delay;
  uint64 context_id_ptr;
  uint64 handler, handler_arg;
  if (argint(0, &delay) < 0)
    return -1;
  if (argaddr(1, &context_id_ptr) < 0)
    return -1;
  if (argaddr(2, &handler) < 0)
    return -1;
  if (argaddr(3, &handler_arg) < 0)
    return -1;

  //TODO: mp3
  struct proc *proc = myproc();
  int context_id;
  copyin(proc->pagetable, (char *)&context_id, context_id_ptr, 4);

  if(context_id == -1){
    context_id = 0;
    while(context_id < MAX_THRD_NUM && proc->timer.context_id_used[context_id])
      context_id ++;
    if(0 <= context_id && context_id < MAX_THRD_NUM){
      proc->timer.context_id_used[context_id] = 1;
      copyout(proc->pagetable, context_id_ptr, (char *)&context_id, 4);
    }
    else
      return -1;
  }

  if(0 <= context_id && context_id < MAX_THRD_NUM){
    proc->timer.context_id = context_id;
    proc->timer.tick = 0;
    proc->timer.delay = delay;
    proc->timer.handler = handler;
    proc->timer.handler_arg = handler_arg;
  }
  else
    return -1;

  return 0;
}

// for mp3
uint64
sys_cancelthrdstop(void)
{
  int context_id, is_exit;
  if (argint(0, &context_id) < 0)
    return -1;
  if (argint(1, &is_exit) < 0)
    return -1;

  //TODO: mp3
  struct proc *proc = myproc();
  if (0 <= context_id && context_id < MAX_THRD_NUM){
    int tick = proc->timer.tick;
    proc->timer.state = 2;
    proc->timer.context_id = context_id;
    proc->timer.tick = 0;
    proc->timer.delay = 0;
    proc->timer.is_exit = is_exit;
    proc->timer.handler = 0;
    proc->timer.handler_arg = 0;
    return tick;
  }
  else
    return -1;
}

// for mp3
uint64
sys_thrdresume(void)
{
  int context_id;
  if (argint(0, &context_id) < 0)
    return -1;

  //TODO: mp3
  struct proc *proc = myproc();
  if(proc->timer.context_id_used[context_id]){
    proc->timer.state = 3;
    proc->timer.context_id = context_id;
  }
  else
    return -1;

  return 0;
}
