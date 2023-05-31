#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
  pid_t child1pid;
  pid_t child2pid;

  child1pid = fork();
  if (child1pid == 0)
  {
    printf("Hi there grownup.  I'm a child!\n");
    return 0;
  }
  else
  {
    child2pid = fork();
    if (child2pid == 0)
    {
      printf("Greetings parent!  I'm your child!\n");
      return 0;
    }
    else
    {
      //1 process!  The original only!
      // int i;
      //for (i = 0; i < 3; i++)
      {
      //There are only two child processes.  Oh my!
	//printf("%d\n", wait(NULL));
      }
      // wait(NULL);
      // wait(NULL);
      printf("I am the parent.  Children!?\n");
      return 0;
    }
  }
}
