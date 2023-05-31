#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int* threadcounts;

// each thread counts how many times a particular value is matched in a
// provided integer array.
// A number is "matched" if it is divisible by the target.
// The result goes in this thread's slot in the global "threadcounts" array.
void* countthem(void* params)
{
  int** args = (int**)params;

  // first parameter is the pointer to the array of values to search
  int* vals = args[0];
  // second parameter is (a pointer to) an integer holding the value
  // to search for
  int searchval = *(args[1]);
  // third parameter is (a pointer to) an integer holding the index
  // that this thread should start from
  int startpos = *(args[2]);
  // fourth parameter is (a pointer to) an integer holding the index
  // that this thread should stop at
  int stoppos = *(args[3]);
  // fifth parameter is (a pointer to) an integer holding my thread id
  // (i.e., my index into the threadcounts array)
  int myid = *(args[4]);

  // initialize my count to 0
  threadcounts[myid] = 0;

  // go through my portion of the array, and count the matches!
  int i;
  for (i = startpos; i < stoppos; i++)
  {
    if (vals[i] % searchval == 0)
      threadcounts[myid]++;
  }

  pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s num_vals search_val\n", argv[0]);
    exit(1);
  }

  // seed the random number generator, so we get the same sequence of numbers
  // each time we run this program
  srand(500);

  // allocate the random numbers (0-999)
  int num_vals = atoi(argv[1]);
  int* random_vals = malloc(num_vals * sizeof(int));
  int i;
  for (i = 0; i < num_vals; i++)
    random_vals[i] = rand() % 1000;

  int search_val = atoi(argv[2]);

  // start time in seconds
  time_t start = time(NULL);

  /*
  int matches = 0;
  for (i = 0; i < num_vals; i++)
  {
    if (random_vals[i] % search_val == 0)
      matches++;
  }
  */

  // let's try 2 threads (very verbose below...could be much more compact!)
  threadcounts = malloc(2 * sizeof(int));
  // get the parameters ready!
  // thread 0
  int t0start = 0;
  int t0stop = num_vals / 2;
  int t0id = 0;
  int* params0[] = {random_vals, &search_val, &t0start, &t0stop, &t0id};
  // thread 1
  int t1start = num_vals / 2;
  int t1stop = num_vals;
  int t1id = 1;
  int* params1[] = {random_vals, &search_val, &t1start, &t1stop, &t1id};

  // launch the threads!
  pthread_t tids[2];
  pthread_create(&tids[0], NULL, countthem, params0);
  pthread_create(&tids[1], NULL, countthem, params1);

  pthread_join(tids[0], NULL);
  pthread_join(tids[1], NULL);

  int matches = threadcounts[0] + threadcounts[1];

  // end time in seconds
  time_t end = time(NULL);

  printf("Matches: %d\n", matches);
  printf("Time: %f\n", difftime(end, start));

  return 0;
}
