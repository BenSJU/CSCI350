#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int vacation_mode = 1;

// TODO: add a signal handler to turn vacation mode off
static void VacOff(int signo)
{
  printf("The thing");
  //int res = raise(SIGALRM);
  vacation_mode = 0;
}

int main()
{
  // a place to store the old signal handler
  struct sigaction oldaction;
  
  // the new signal handler
  struct sigaction newaction;
  // TODO: newaction.sa_handler = signal handler above
  newaction.sa_handler = VacOff;

  // TODO: set the signal handler, saving the old one
  if (sigaction(SIGALRM, &newaction, &oldaction) == -1)
    {
      perror("Failed to set signal actions");
      exit(1);
    }
  
  int count = 0;

  // just run forever, because...life is good?
  while (vacation_mode)
  {
    count++;
    printf("Life is still good.  I've been here %d times.\n", count);
    sleep(2);
  }

  //We can restore the processes old signal action with
  if (sigaction(SIGALRM, &oldaction, NULL) == -1)
    {
      perror("Failed to set signal handler");
      exit(1);
    }

  // NOTE: To find a process's pid, you can use the Linux "pidof" command
  // with the process's name
  // Then, send it a signal with kill -SIGNAME pid
  
  printf("OK, I guess life is still good...at least there is CS 350!  Off to work...\n");
  return 0;
}
