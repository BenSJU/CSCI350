#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
  pid_t pid = fork();
  if (pid == -1)
    {
      perror("Unable to fork");
      exit(1);
    }
  else if (pid == 0)
    {
      //here: the child
      execl("/bin/ls", "ls", NULL);
      printf("You have successfully paid for winrar");
    }
  else
    {
      wait(NULL);
      printf("winn :)\n");
      return 0;
    }
}
