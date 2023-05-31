#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{

  if (argc == 1)
    {
      fprintf(stderr, "Usage: %s prog [args...]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  
  //command argument for execvp
  char* command = argv[1];
  
  // capture the start time
  time_t start = time(NULL);

  // fork the child process
  pid_t pid = fork();

  //status pointer for wait functions
  int status;

  if (pid == -1)
  {
    perror("Failed to fork child process");
    exit(1);
  }
  if (pid == 0)
    {
      execvp(command, &argv[1]);
      perror("Could not exec child");
      //return 1;
      exit(EXIT_FAILURE);
    }

  pid_t waitpid;
  waitpid = wait(&status);
  if (waitpid == -1)
    {
      perror("Failed to wait for child\n");
    }
  else if (WIFEXITED(status) && !WEXITSTATUS(status))
    {
      //wait(NULL);
      time_t end = time(NULL);
      double diff_t = difftime(end, start);
      printf("Time in seconds: %d\n", (int) diff_t);
    }
  // else if (WIFEXITED(status) || WIFSIGNALED(status) || WIFSTOPPED(status))
  // {
  //   perror("Could not exec child");
  // }

  //double diff_t = difftime(end, start);

  //printf("Time in seconds: %f\n", diff_t);

  return 0;
}
