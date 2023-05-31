#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
  if (argc != 3)
    {
      fprintf(stderr, "Usage: %s cmd1 cmd2", argv[0]);
      exit(1);
    }

  //Pids for children
  pid_t child_a, child_b;

  child_a = fork();

  if (child_a == -1)
    {
      perror("Failed to fork child process");
      exit(1);
    }
  else if (child_a == 0)
    {
      execlp(argv[1], argv[1], (char*)NULL);
      perror("Failed to exec");
      exit(EXIT_FAILURE);
    }
  else
    {
      waitpid(child_a, NULL, 0);
      printf("Program 1 done!\n");
      child_b = fork();
      if (child_b == -1)
	{
	  perror("Failed to fork child process");
	  exit(1);
	}
      else if (child_b == 0)
	{
	  execlp(argv[2], argv[2], (char*)NULL);
	  perror("Failed to exec");
	  exit(EXIT_FAILURE);
	}
    }
  
  // waitpid(child_a, NULL, 0);
  waitpid(child_b, NULL, 0);
  printf("Program 2 done!\n");
  
  // printf("Successfully returned to parent\n");

  return 0;
}
