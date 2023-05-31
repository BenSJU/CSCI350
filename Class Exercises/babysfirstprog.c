#include <stdio.h>

int main()
{
  int mypid = getpid();

  printf("Hello, world!\n");
  printf("My process id is: %d %d\n", mypid, &mypid);

  return 0;
}
