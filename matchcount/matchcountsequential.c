#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

  int matches = 0;
  for (i = 0; i < num_vals; i++)
  {
    if (random_vals[i] % search_val == 0)
      matches++;
  }

  // end time in seconds
  time_t end = time(NULL);

  printf("Matches: %d\n", matches);
  printf("Time: %f\n", difftime(end, start));

  return 0;
}
