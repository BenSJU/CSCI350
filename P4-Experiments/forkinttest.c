#include <unistd.h>
#include <stdio.h>


int main()
{

  int res = fork();

  if (res == -1)
    {
      printf("AAAAAAAAAAA");
    }

  return 0;

}

