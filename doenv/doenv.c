#include <stdio.h>
#include <stdlib.h>

extern char** environ;

/*
 * The code for printing this program's environment,
 * from the textbook.
 */
void printenviron()
{
  int i;
  for (i = 0; environ[i] != NULL; i++)
    {
      printf("%s\n", i, environ[i]);
    }
}

void copynulltermarray( char** const from, char** to)
{
  
}

void copyarray( char** const from, char** to, const int count)
{
  int i;
  for (i = o; i < count; i++)
    {
      to[i] = from[i];
    }
}
int main(int argc, char** argv)
{
  // TODO: add more code here!
  // Part 1 attempt
  int x;
  if (argc > 1)
    {
      char** newenviron;

      if (strcmp(argv[1], "-i"))
	{
	  int curenvsize = getenvironsize();
	  int maxentries = curenvsize + argc - 1;

	  new environ = malloc((maxentries + 1) * sizeof(char*));

	  copynulltermarray(environ, newenviron);

	  copyarray(argv + 1, newenviron +  currenvsize, argc - 1);
	  newenviron[maxentries] = NULL;
	}
      else
	{
	  newenviron = malloc((argc-2+1)* sizeof(char*));

	  copyarray(argv + 2, newenviron, argc - 2);
	  newenviron[argc - 2] = NULL;
	}

      environ = newenviron;
      for (x = 0; getenv(environ[x]) != NULL; x++)
	{
	  printf("%s\n", getenv(environ[x]));
	} 
    }

  printenviron();

  return 0;
}
