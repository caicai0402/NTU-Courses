#include "param.h"
#include "types.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"
#include "proc.h"

/* NTU OS 2023 */
/* Page fault handler */
int handle_pgfault() {
  /* TODO */
  /* Find the address that caused the fault */
  struct proc *p = myproc();
  uint64 va = PGROUNDDOWN(r_stval()), pa = (uint64)kalloc();
  pte_t *pte = walk(p->pagetable, va, 1);

  if(pa == 0)
    panic("paging: kalloc");

  if(*pte & PTE_S) {
    begin_op();
    read_page_from_disk(ROOTDEV, (char *)pa, PTE2BLOCKNO(*pte));
    end_op();
    *pte = PA2PTE(pa) | PTE_FLAGS(*pte);
    *pte &= ~PTE_S;
    *pte |= PTE_V;
  } 
  else {
    memset((void *)pa, 0, PGSIZE);
    if(mappages(p->pagetable, va, PGSIZE, pa, PTE_R | PTE_U | PTE_X | PTE_W) != 0) {
      kfree((void *)pa);
      panic("paging: mappages");
    }
  }

  return 0;
}
