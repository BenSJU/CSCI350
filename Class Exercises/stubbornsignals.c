#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

// TODO: make a signal handler function
static void nicetry(int signo)
{
  printf("Perish\n");
}

int main()
{
  printf("I am a very stubborn process.  Go ahead; try to terminate me!\n");

  // TODO: a struct to store the new signal handler...to show how we don't care
  // MWUHAHAHA
  struct sigaction newaction;
  newaction.sa_handler = nicetry;

  // TODO: set up the sig actions!
  if (sigaction(SIGINT, &newaction, NULL) == -1)
    {
      perror("Failed to set signal action");
      exit(1);
    }
  if (sigaction(SIGTERM, &newaction, NULL) == -1)
    {
      perror("Failed to set signal action");
      exit(1);
    }
  //this will pass error
  if (sigaction(SIGKILL, &newaction, NULL) == -1)
    {
      perror("Failed to set signal action");
      exit(1);
    }
  
  


  // run forever.  No one can stop me!
  while (1) ;
}
