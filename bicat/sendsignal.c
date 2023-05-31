#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

int main(int pid)
{
  int ret;
  ret = kill(pid, SIGUSR1);
  printf("ret : %d", ret);

  return 0;

}
