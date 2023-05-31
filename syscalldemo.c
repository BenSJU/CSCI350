#include <sys/syscall.h>
#include <stdio.h>

int main()
{
  printf("getpid syscall number: %d\n", SYS_getpid);

  int pid;

  // directly using x86 assembly
  asm("movl $20, %eax;");
  asm("int $0x80;");
  asm("movl %%eax, %0" : "=r"(pid));
  printf("pid: %d\n", pid);

  // using syscall wrapper
  syscall(20, &pid);
  printf("pid: %d\n", pid);

  // using the "regular" system call wrapper wrapper function
  pid = getpid();
  printf("pid: %d\n", pid);
}
