#include "types.h"
#include "stat.h"
#include "user.h"
#include "mmu.h"

#define N (8 * (1 << 20))

// Use constants from mmu.h
#define PGSIZE PTE_SZ
#define MAXVA UADDR_SZ
typedef uint pte_t;

// PTE macros for ARM
#define PTE_FLAGS(pte) ((pte) & 0xFFF)
#define PTEVA2PA(pte, va) (((pte) & ~0xFFF) | ((va) & 0xFFF))
#define SUPERPGROUNDUP(sz) (((sz) + 512 * PGSIZE - 1) & ~(512 * PGSIZE - 1))

void print_pt();
void print_kpt();
void ugetpid_test();
void superpg_test();

int main(int argc, char *argv[])
{
  print_pt();
  ugetpid_test();
  print_kpt();
  superpg_test();
  printf(1, "pttest: all tests succeeded\n");
  exit();
}

char *testname = "";

void err(char *why)
{
  printf(1, "pttest: %s failed: %s, pid=%d\n", testname, why, getpid());
  exit();
}

void print_pte(uint va)
{
  pte_t pte = (pte_t)pgpte((void *)va);
  printf(1, "va: 0x%x | pte: 0x%x | pa: 0x%x | perm: 0x%x\n", va, pte, PTEVA2PA(pte, va), PTE_FLAGS(pte));
}

void print_pt()
{
  printf(1, "print_pt starting\n");
  for (uint i = 0; i < 10; i++)
  {
    print_pte(i * PGSIZE + 0xDEF); // offset within a page
  }
  uint top = MAXVA / PGSIZE;
  for (uint i = top - 10; i < top; i++)
  {
    print_pte(i * PGSIZE + 0xDEF);
  }
  printf(1, "print_pt: OK\n");
}

void ugetpid_test()
{
  int i;

  printf(1, "ugetpid_test starting\n");
  testname = "ugetpid_test";

  for (i = 0; i < 64; i++)
  {
    int ret = fork();
    if (ret != 0)
    {
      wait();
      continue;
    }
    if (getpid() != ugetpid())
      err("missmatched PID");
    exit();
  }
  printf(1, "ugetpid_test: OK\n");
}

void print_kpt()
{
  printf(1, "print_kpt starting\n");
  kpt(); // Implement in vm.c to access kernel pagetable
  printf(1, "print_kpt: OK\n");
}

void supercheck(uint s)
{
  pte_t last_pte = 0;

  for (uint p = s; p < s + 512 * PGSIZE; p += PGSIZE)
  {
    pte_t pte = (pte_t)pgpte((void *)p);
    if (pte == 0)
      err("no pte");
    if ((uint)last_pte != 0 && pte != last_pte)
    {
      err("pte different");
    }
    // if ((pte & PTE_V) == 0 || (pte & PTE_R) == 0 || (pte & PTE_W) == 0)
    // {
    //   err("pte wrong");
    // }
    last_pte = pte;
  }

  for (int i = 0; i < 512; i += PGSIZE)
  {
    *(int *)(s + i) = i;
  }

  for (int i = 0; i < 512; i += PGSIZE)
  {
    if (*(int *)(s + i) != i)
      err("wrong value");
  }
}

// This test will always fail as XV6 does not support superpages
void superpg_test()
{
  int pid;

  printf(1, "superpg_test starting\n");
  testname = "superpg_test";

  char *end = sbrk(N);
  if (end == 0 || end == (char *)0xffffffffffffffff)
    err("sbrk failed");

  uint s = SUPERPGROUNDUP((uint)end);
  supercheck(s);
  if ((pid = fork()) < 0)
  {
    err("fork");
  }
  else if (pid == 0)
  {
    supercheck(s);
    exit();
  }
  else
  {
    wait();
  }
  printf(1, "superpg_test: OK\n");
}
