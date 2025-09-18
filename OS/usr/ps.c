#include "types.h"
#include "stat.h" 
#include "user.h"

int main(int argc, char *argv[])
{
    int proc_count = getprocscount(); // Get the number of active processes
    char name[32];
    char state[32];

    printf(1, "PID\tPPID\tNAME\tSTATE\t   SYSCALL_COUNT\n");
    printf(1, "-----------------------------------------------\n");

    for(int i = 0; i < proc_count; i++) {
        int pid = getprocid(i); // Get the process ID of the ith active process
        int ppid = getparentpid(i); // Get the parent process ID of the ith active process
        getname(i, name); // Get the name of the ith active process
        getstate(i, state); // Get the state of the ith active process
        int syscall_count = getsyscallcount(i); // Get the syscall count of the ith active process

        printf(1, "%d\t%d\t%s\t%s\t%d\n", pid, ppid, name, state, syscall_count);
    }

    exit();
}