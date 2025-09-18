#include "types.h"
#include "arm.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "pstat.h"
#include "spinlock.h"

extern struct {
    struct spinlock lock;
    struct proc proc[NPROC];
} ptable;

int sys_fork(void)
{
    return fork();
}

int sys_exit(void)
{
    exit();
    return 0; // not reached
}

int sys_wait(void)
{
    return wait();
}

int sys_kill(void)
{
    int pid;

    if (argint(0, &pid) < 0)
    {
        return -1;
    }

    return kill(pid);
}

int sys_getpid(void)
{
    return proc->pid;
}

int sys_sbrk(void)
{
    int addr;
    int n;

    if (argint(0, &n) < 0)
    {
        return -1;
    }

    addr = proc->sz;

    if (growproc(n) < 0)
    {
        return -1;
    }

    return addr;
}

int sys_sleep(void)
{
    int n;
    uint ticks0;

    if (argint(0, &n) < 0)
    {
        return -1;
    }

    acquire(&tickslock);

    ticks0 = ticks;
    proc->boostsleft += n;

    while (ticks - ticks0 < n)
    {
        if (proc->killed)
        {
            release(&tickslock);
            proc->duration_of_sleep = 0;
            return -1;
        }

        // Here we will include the condition to update duration of sleep variable
        // It is equal to number of ticks when process should wake up
        proc->duration_of_sleep = ticks0 + n;

        sleep(&ticks, &tickslock);
    }

    release(&tickslock);
    return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
    uint xticks;

    acquire(&tickslock);
    xticks = ticks;
    release(&tickslock);

    return xticks;
}

// gets the number of active processes so that ps.c knows how many times to run the loop
int sys_getprocscount(void)
{
    return getprocscount();
}

// gets the process ID of the nth active process
int sys_getprocid(void)
{
    int n;
    struct proc *p;

    if (argint(0, &n) < 0 || (p = getproc(n)) == 0)
    {
        return -1;
    }

    return p->pid;
}

// gets the parent process ID of the nth active process
int sys_getparentpid(void)
{
    int n;
    struct proc *p;

    if (argint(0, &n) < 0 || (p = getproc(n)) == 0)
    {
        return -1;
    }

    if (p->parent != NULL)
    {
        return p->parent->pid;
    }

    return 0; // Return 0 if there is no parent
}

// gets the name of the nth active process
int sys_getname(void)
{
    int n;
    char *name;
    struct proc *p;

    if (argint(0, &n) < 0 || argptr(1, &name, 16) < 0 || (p = getproc(n)) == 0)
    {
        return -1;
    }
    strncpy(name, p->name, 16);
    return 0;
}

// gets the state of the nth active process
int sys_getstate(void)
{
    int n;
    char *state;
    struct proc *p;

    if (argint(0, &n) < 0 || argptr(1, &state, 32) < 0 || (p = getproc(n)) == 0)
    {
        return -1;
    }

    switch (p->state)
    {
    case UNUSED:
        strncpy(state, "UNUSED  ", 32);
        break;
    case EMBRYO:
        strncpy(state, "EMBRYO  ", 32);
        break;
    case SLEEPING:
        strncpy(state, "SLEEPING", 32);
        break;
    case RUNNABLE:
        strncpy(state, "RUNNABLE", 32);
        break;
    case RUNNING:
        strncpy(state, "RUNNING ", 32);
        break;
    case ZOMBIE:
        strncpy(state, "ZOMBIE  ", 32);
        break;
    default:
        strncpy(state, "UNKNOWN ", 32);
        break;
    }

    return 0;
}

// gets the syscall count of the nth active process
int sys_getsyscallcount(void)
{
    int n;
    struct proc *p;

    if (argint(0, &n) < 0 || (p = getproc(n)) == 0)
    {
        return -1;
    }

    return p->syscall_count;
}

int sys_settickets(void)
{
    int pid, n_tickets;
    if (argint(0, &pid) < 0 || argint(1, &n_tickets) < 0 || n_tickets <= 0)
        return -1;
    struct proc *p;
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        cprintf("kernel pid: %d\n", p->pid);
        if (p->pid == pid) {
            p->tickets = n_tickets;
            return 0;
        }
    }
    return -1; // pid not found
}

extern uint rseed; // for srand

int sys_srand(void)
{
    uint seed;
    if (argint(0, (int*)&seed) < 0)
        return -1;
    rseed = seed;
    return 0;
}

int sys_getpinfo(void)
{
    struct pstat *ps;
    if (argptr(0, (void*)&ps, sizeof(*ps)) < 0)
        return -1;
    struct proc *p;
    int i = 0;
    for (p = ptable.proc; p < &ptable.proc[NPROC]; p++, i++) {
        ps->inuse[i] = (p->state != UNUSED);
        ps->pid[i] = p->pid;
        ps->tickets[i] = p->tickets;
        ps->runticks[i] = p->runticks;
        ps->boostsleft[i] = p->boostsleft;
    }
    return 0;
}

int sys_pgpte(void)
{
    void *va;
    if (argptr(0, (char**)&va, sizeof(void*)) < 0)
        return -1;
    
    // Call the walkpgdir function to get the PTE for the virtual address
    pte_t *pte = walkpgdir(proc->pgdir, va, 0);
    
    if (pte == 0)
        return 0; // Return 0 if no PTE exists
    
    return (int)*pte; // Return the PTE value
}

int sys_ugetpid(void)
{
    return proc->pid;
}

int sys_kpt(void)
{
    kpt();
    return 0;
}