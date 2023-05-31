#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int counter;
pthread_mutex_t thelock = PTHREAD_MUTEX_INITIALIZER;

void* dostuff(void* params)
{
  // pthread_mutex_lock(&thelock);
  while (1)
  {
    pthread_mutex_lock(&thelock);
    if (counter % 2 == 0)
    {
      printf("Checking counter...\n");
      printf("Counter is now: %d\n", counter);
    }
    pthread_mutex_unlock(&thelock);
  }
}

int main()
{
  counter = 0;

  // TODO: create the thread
  pthread_t tid;
  if (pthread_create(&tid, NULL, dostuff, NULL) != 0)
    {
      perror("Failed to create thread");
    }

  while (counter < 100000)
  {
    pthread_mutex_lock(&thelock);
    counter++;
    pthread_mutex_unlock(&thelock);
  }

  // TODO: kill (cancel) the thread before exiting
  if(pthread_cancel(tid) != 0)
    {
      perror("Failed to cancel thread");
    }

  return 0;
}
