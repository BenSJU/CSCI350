#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s output_prog input_prog\n", argv[0]);
    exit(1);
  }

  int pipefds[2];
  // TODO: make the pipe!
  if (pipe(pipefds) == -1)
    {
      perror("Failed to create pipe");
      exit(1);
    }

  pid_t outputchildpid = fork();
  if (outputchildpid == -1)
  {
    perror("Fork for output child failed");
    exit(1);
  }
  else if (outputchildpid == 0)
  {
    // here: the child
    // TODO: redirect my standard output to one end of the pipe before exec
    dup2(pipefds[1], STDOUT_FILENO);

    // NOTE: We should close the old stdout here!
    close(pipefds[1]);
    close(pipefds[0]);

    // now run the program!  (Note: for simplicity, doesn't handle arguments)
    execlp(argv[1], argv[1], NULL);
    perror("Failed to exec the output child");
    exit(1);
  }

  // here: the parent
  pid_t inputchildpid = fork();
  if (inputchildpid == -1)
  {
    perror("Fork for input child failed");
    exit(1);
  }
  else if (inputchildpid == 0)
  {
    // here: the child
    // TODO: redirect one end of the pipe to my standard input before exec
    dup2(pipefds[0], STDIN_FILENO);
    
    // NOTE: We should close the old stdin here!
    close(pipefds[0]);
    close(pipefds[1]);

    // now run the program!  (Note: for simplicity, doesn't handle arguments)
    execlp(argv[2], argv[2], NULL);
    perror("Failed to exec the input child");
    exit(1);
  }

  // here: the parent
  // NOTE: We should really close the pipe here...
  // wait for both children, then exit
  wait(NULL);
  wait(NULL);
  return 0;
}
