#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  char buf[6];

  // read up to 5 bytes into "buf"
  // NOTE: Using proper error handling for "read"!
  int bytesread;
  while ((bytesread = read(STDIN_FILENO, buf, 5)) == -1 && errno == EINTR) ;
  if (bytesread == -1)
  {
    perror("Problem reading from stdin somehow");
    exit(EXIT_FAILURE);
  }

  // our first version...using printf instead
  // NOTE: We need to null-terminate the buffer!
  // buf[bytesread] = '\0';
  // printf("I read: %s\n", buf);

  // write the contents of the buffer to standard output's file descriptor
  // NOTE: Using proper error handling for "read"!
  int byteswritten;
  while ((byteswritten = write(STDOUT_FILENO, buf, bytesread)) == -1 && errno == EINTR) ;
  if (byteswritten == -1)
  {
    perror("Problem writing to stdout somehow");
    exit(EXIT_FAILURE);
  }

  return 0;
}
