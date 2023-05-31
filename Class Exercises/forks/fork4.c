#include <unistd.h>
#include <stdio.h>

int main()
{
  int i;
  for (i = 0; i < 4; i++)
  {
    pid_t pid = fork();
    if (pid == 0)
    {
      printf("I am a child! %d\n", pid);
      break;
    }
    else
    {
      printf("I am its parent! %d\n", pid);
    }
  }

  printf("Program ending for %d!\n", getpid());

  return 0;
}
