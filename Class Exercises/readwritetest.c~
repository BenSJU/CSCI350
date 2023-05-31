#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main()
{
  char buf[6];
 
  // read up to 5 bytes into "buf"
  int bytesread;
  while((bytesread = read(STDIN_FILENO, buf, 5)) == -1 && errno == EINTR)
    ;
  if (bytesread == -1)
    {
      perror("Problem reading from stdin somehow");
      exit(EXIT_FAILURE);
    }
  buf[bytesread] = '\0';

  int byteswritten;
  while ((byteswritten = write(STDOUT_FILENO, buf, bytesread)) == -1 && errno == EINTR)
    ;
  if (byteswritten == -1)
    {
      perror("Pronlem writing to stdout somehow");
      exit(EXIT_FAILURE);
    }

  return 0;
  
 
}
