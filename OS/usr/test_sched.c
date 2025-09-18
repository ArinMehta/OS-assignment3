#include "types.h"
#include "user.h"
#include "pstat.h"

#define MAX_CHILDREN 8
#define WORK_LOOP 100000000

void print_proc_info(const char *msg) {
    struct pstat ps;
    if (getpinfo(&ps) == 0) {
        printf(1, "\n%s\n", msg);
        for (int i = 0; i < NPROC; i++) {
            if (ps.inuse[i]) {
                printf(1, "pid: %d, tickets: %d, runticks: %d, boostsleft: %d, inuse: %d\n",
                    ps.pid[i], ps.tickets[i], ps.runticks[i], ps.boostsleft[i], ps.inuse[i]);
            }
        }
    }
}

void child_work(int tickets, int sleep_ticks1, int run_ticks, int sleep_ticks2, int id) {
    int pid = getpid();
    settickets(pid, tickets);
    printf(1, "Child %d (pid %d): tickets=%d, sleep1=%d, run=%d, sleep2=%d\n", id, pid, tickets, sleep_ticks1, run_ticks, sleep_ticks2);

    // First sleep
    if (sleep_ticks1 > 0) {
        printf(1, "Child %d sleeping for %d ticks...\n", id, sleep_ticks1);
        sleep(sleep_ticks1);
        printf(1, "Child %d woke up!\n", id);
        print_proc_info("Process info after waking up (first sleep):");
    }

    // Simulate work for `run_ticks`
    for (volatile int i = 0; i < WORK_LOOP; i++) {
        if (i % (WORK_LOOP / 10) == 0) {
            printf(1, "Child %d (pid %d) progress: %d%%\n", id, pid, (i * 100) / WORK_LOOP);
        }
        if (i >= (WORK_LOOP * run_ticks) / 10) {
            break; // Simulate running for `run_ticks`
        }
    }

    // Second sleep
    if (sleep_ticks2 > 0) {
        printf(1, "Child %d sleeping for %d ticks...\n", id, sleep_ticks2);
        sleep(sleep_ticks2);
        printf(1, "Child %d woke up!\n", id);
        print_proc_info("Process info after waking up (second sleep):");
    }

    // Simulate remaining work
    for (volatile int i = 0; i < WORK_LOOP; i++) {
        if (i % (WORK_LOOP / 10) == 0) {
            printf(1, "Child %d (pid %d) progress: %d%%\n", id, pid, (i * 100) / WORK_LOOP);
        }
    }

    printf(1, "Child %d (pid %d) finished work\n", id, pid);
    print_proc_info("Process info after finishing work:");
    exit();
}

int main(void) {
    int nchildren = 6; // Number of child processes
    int tickets[MAX_CHILDREN] = {1, 2, 4, 8, 16, 32}; // Tickets for each child
    int sleep_ticks1[MAX_CHILDREN] = {3, 0, 5, 0, 7, 0}; // First sleep ticks for each child
    int run_ticks[MAX_CHILDREN] = {2, 0, 3, 0, 4, 0}; // Run ticks after first sleep
    int sleep_ticks2[MAX_CHILDREN] = {4, 0, 6, 0, 8, 0}; // Second sleep ticks for each child

    printf(1, "Parent PID: %d\n", getpid());
    settickets(getpid(), 1);

    for (int i = 0; i < nchildren; i++) {
        int pid = fork();
        if (pid == 0) {
            child_work(tickets[i], sleep_ticks1[i], run_ticks[i], sleep_ticks2[i], i);
        }
    }

    print_proc_info("Process info before waiting for children:");

    // Parent waits for all children
    for (int i = 0; i < nchildren; i++) wait();

    print_proc_info("Process info after test:");

    exit();
}