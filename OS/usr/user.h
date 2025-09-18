#include "pstat.h"

struct stat;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(char*, int);
int mknod(char*, short, short);
int unlink(char*);
int fstat(int fd, struct stat*);
int link(char*, char*);
int mkdir(char*);
int chdir(char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
// Adding new system calls to connect kernel and user space for ps command
int getprocscount(void);
int getprocid(int ind); // gets the process ID of the process at index ind
int getparentpid(int ind); // gets the parent process ID of the process at index ind
int getname(int ind, char* buffer); // gets the name of the process at index ind
int getstate(int ind, char* buffer); // gets the state of the process at index ind
int getsyscallcount(int ind); // gets the syscall count of the process at index ind
// System calls for scheduling and process management
int settickets(int pid, int n_tickets);
void srand(uint seed);
int getpinfo(struct pstat *);
int pgpte(void *va);
int ugetpid(void);
int kpt(void); // System call to print first 10 and last 10 entried of the kernel page table

// ulib.c
int stat(char*, struct stat*);
char* strcpy(char*, char*);
void *memmove(void*, void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, char*, ...);
char* gets(char*, int max);
uint strlen(char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
