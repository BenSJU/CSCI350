#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static int do_count = 1;
static void setdocount(int signo)
{
  if (signo == 2)
    {
      printf("Counter stopped\n");
      do_count = 0;
    }
  else if (signo == 14)
    {
      printf("Counter started\n");
      do_count = 1;
    }
  //do_count = 0;
}

int main()
{

  struct sigaction newaction;
  newaction.sa_handler = setdocount;

  if (sigaction(SIGINT, &newaction, NULL) == -1)
    {
      perror("Failed to set the signal handler");
      exit(1);
    }
  if (sigaction(SIGALRM, &newaction, NULL) == -1)
    {
      perror("Failed to set the signal handler");
      exit(1);
    }

  int counter = 0;
  while(1)
    {
      //stop counting when the global variable gets turned off,
      //but keep the program running!
      if (do_count == 1)
	{
	  counter++;
	  printf("%d\n", counter);
	  sleep(1);

	}
    }  
}
