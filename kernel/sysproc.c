#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "process.h"
#include "pstat.h"

extern struct process_control_block process_table[NPROC];

uint64 sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
}

uint64 sys_getpid(void)
{
  return myproc()->pid;
}

uint64 sys_fork(void)
{
  return fork();
}

uint64 sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64 sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64 sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64 sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64 sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
/*
    > Return address obtained from argaddr()
    > Iterates over process table, taking desired information from each
    > Return the pstat structure to the address
*/
uint64 sys_getpinfo(void)
{
  struct pstat ps;
  uint64 dst;
  argaddr(0,&dst);
  
  struct process_control_block *p;
  int i = 0;
  for(p = process_table; p < &process_table[NPROC]; p++) 
  {
    acquire(&p->lock);
    
    ps.inuse[i] = 1;
    strncpy(ps.name[i],p->name,16);
    ps.state[i] = p->state;
    ps.pid[i] = p->pid;
    ps.real_priority[i] = p->real_priority;
    
    release(&p->lock);
    i++;
  }
  either_copyout(1,dst, (void*)&ps,sizeof(struct pstat));
  return 0;
}
/*
    > This system call only recieves the desired priority
    > It's assumed that it will be called from the desired process
    > Priority is tested for correct range
    > myproc() is used to obtain current process
    > p->real_priority set to the desired priority
*/
uint64 sys_setPriority(void)
{
  int prio;
  argint(0, &prio);

  if(prio < -20 || prio > 20)
    return -1;
  struct process_control_block *p = myproc();
  acquire(&p->lock);
  p->real_priority = prio;
  release(&p->lock);
  return 0;
}
/*
    > I was a bit confused with the implementation of this system call
    > It recieves the pid and the intended priority so I opted to use 
      the pid via searching the process table
    > Once the process pid is found and the state is not UNUSED set prio    
*/
uint64 sys_setEffectivePriority(void)
{
  int pid;
  int prio;

  argint(1, &prio);
  if(prio < -20 || prio > 20)
    return -1;
  
  argint(0, &pid);
  struct process_control_block *p;

  for(p = process_table; p < &process_table[NPROC]; p++) 
  {
    if(p->pid == pid && p->state != UNUSED)
    {
      acquire(&p->lock);
      p->effective_priority = prio;
      release(&p->lock);
      return 0;
    }
  }
  return -1;
}
