#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// global variable for expected sum, to avoid passing a second parameter to
// the pthread function
int expectedsum;

// a naive recursive implementation of subset sum
// "set" is the array of numbers from the file
// "n" is the number of elements in "set"
// "sum" is the target sum
// source: https://www.geeksforgeeks.org/c-program-for-subset-sum-problem-dp-25/
int isSubsetSum(int set[], int n, int sum)
{
  // Base Cases
  if (sum == 0)
    return 1; // "true" in C terms
  if (n == 0 && sum != 0)
    return 0; // "false" in C terms
  
  // If last element is greater than sum, then ignore it
  if (set[n - 1] > sum)
    return isSubsetSum(set, n - 1, sum);
  
  /* else, check if sum can be obtained by any of the following
     (a) including the last element
     (b) excluding the last element   */
  return isSubsetSum(set, n - 1, sum) || 
         isSubsetSum(set, n - 1, sum - set[n - 1]);
}

// the function to run in the threads!
void* readfileandsolve(void* params)
{
  // one way to pass the file path...feel free to change if you don't like it!
  char* path = (char*) params;

  // TODO: modify the code below to completely solve the problem!

  // open the file, using POSIX file reading
  FILE* f = fopen(path, "r");
  if (f == NULL)
  {
    fprintf(stderr, "Failed to open file %s: ", path);
    perror(NULL);
    return NULL;
  }

  // for now, read just the first number in the file...TODO: update this!
  int nums[1000];
  int i;
  for (i = 0; i < 1000; i++)
    {
      int res = fscanf(f, "%d", &nums[i]);
      if (res != 1 && ferror(f))
	{
	  fprintf(stderr, "Failed to read from file %s: ", path);
	  perror(NULL);
	  fclose(f);
	  return NULL;
	}
      if (res == EOF)
	{
	  break;
	}

    }

  // see documentation: stream is "shut down" even if this call fails
  fclose(f);

  if (isSubsetSum(nums, i, expectedsum))
    printf("Sum found in file %s\n", path);
  
  return NULL;
}

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s searchN [file1 file2 ...]\n", argv[0]);
    exit(1);
  }

  // fill in the expected sum...shared memory hooray!
  expectedsum = atoi(argv[1]);

  // loop through each file path and run subset sum!
  // TODO: make this run in parallel!
  int i;
  for (i = 2; i < argc; i++)
  {
    readfileandsolve(argv[i]);
  }

  return 0;
}
