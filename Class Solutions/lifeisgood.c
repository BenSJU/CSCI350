#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int vacation_mode = 1;

// TODO: add a signal handler to turn vacation mode off

int main()
{
  // a place to store the old signal handler
  struct sigaction oldaction;
  
  // the new signal handler
  struct sigaction newaction;
  // TODO: newaction.sa_handler = signal handler above

  // TODO: set the signal handler, saving the old one
  
  int count = 0;

  // just run forever, because...life is good?
  while (vacation_mode)
  {
    count++;
    printf("Life is still good.  I've been here %d times.\n", count);
    sleep(2);
  }

  // NOTE: To find a process's pid, you can use the Linux "pidof" command
  // with the process's name
  // Then, send it a signal with kill -SIGNAME pid
  
  printf("OK, I guess life is still good...at least there is CS 350!  Off to work...\n");
  return 0;
}
