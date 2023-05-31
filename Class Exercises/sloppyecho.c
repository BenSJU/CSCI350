#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  //read some chars from keyboard
  char buf[100];
  int bytesread = read(0, buf, 100);
  if (bytesread == -1)
    {
      perror("Failed to read from stdin (echo)");
      exit(1);
    }

  //print them back
  printf("%s\n", buf);

  return 0;
}
