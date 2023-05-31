#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s prog input_to_send\n", argv[0]);
    exit(1);
  }

  // TODO: make a "pipe"!
  int pipefds[2];
  if (pipe(pipefds) == -1)
    {
      perror("Failed to create pipe");
      exit(1);
    }
  //close(0);  // >:)

  pid_t childpid = fork();
  if (childpid == -1)
  {
    perror("Fork failed");
    exit(1);
  }
  else if (childpid != 0)
  {
    // here: the parent
    int size = strlen(argv[2]);

    // TODO: write to the pipe!
    write(pipefds[1], argv[2], size);

    wait(NULL);
    return 0;
  }
  else
  {
    // here: the child

    // TODO: make STDIN for the program come from the pipe!
    dup2(pipefds[0], STDIN_FILENO);

    // now run the program!
    execlp(argv[1], argv[1], NULL);
    perror("Failed to exec the child!");
    exit(1);
  }

  return 0;
}
