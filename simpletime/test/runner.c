// A program that runs for (almost) exactly N seconds
// where N is specified by the environment variable TIMETORUN
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// to force optimization to keep the loop
int global = 1;

int main()
{
  char* timestr = getenv("TIMETORUN");
  if (timestr == NULL)
  {
    fprintf(stderr, "Missing environment variable TIMETORUN\n");
    exit(1);
  }
  int timetorun = atoi(timestr);
  if (timetorun == 0)
  {
    fprintf(stderr, "Invalid TIMETORUN\n");
    exit(1);
  }
  
  time_t start = time(NULL);

  while (difftime(time(NULL), start) < timetorun)
  {
    int i;
    for (i = 0; i < 10000; i++)
      global += global;
  }

  printf("All done!\n");

  return 0;
}
