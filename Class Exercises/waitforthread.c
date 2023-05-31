#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Add the two values in the int array
// (passed in params) and put the result
// in the first slot of that array
void* sumthem(void* params)
{
  //add code here!
  printf("I'm the thread\n");
  int* things = (int*)params;

  things[0] = things[0] + things[1];

  pthread_exit(NULL);

}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage %s val1 val2\n", argv[0]);
    exit(1);
  }

  int vals[2];
  vals[0] = atoi(argv[1]);
  vals[1] = atoi(argv[2]);

  // TODO: create a thread and run "sumthem" (above) to add
  // the vals together
  pthread_t tid;
  if (pthread_create(&tid, NULL, sumthem, vals) != 0)
    {
      perror("Failed to create thread");
      exit(1);
    }

  // TODO: wait for the thread to finish before printing the result!
  if (pthread_join(tid, NULL) != 0)
    {
      perror("Failed to join with child thread");
      exit(1);
    }

  printf("Got the following value back: %d\n", vals[0]);
  return 0;
}
