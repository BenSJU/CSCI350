#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char** environ;

void printenviron()
{
  int i;
  for (i = 0; environ[i] != NULL; i++)
  {
    printf("%s\n", environ[i]);
  }
}

int getenvironsize()
{
  int result = 0;
  for (result = 0; environ[result] != NULL; result++) ;
  return result;
}

void copynulltermarray(char** const from, char** to)
{
  int i;
  for (i = 0; from[i] != NULL; i++)
    to[i] = from[i];
}

void copyarray( char** const from, char** to, const int count)
{
  int i;
  for (i = 0; i < count; i++)
    to[i] = from[i];
}

int main(int argc, char** argv)
{
  if (argc > 1)
  {
    char** newenviron;
    
    if (strcmp(argv[1], "-i"))
    {
      // this is the harder case...
      // start by figuring out the max size we'll need, based on
      // the current environ size and the size of argv
      int curenvsize = getenvironsize();
      int maxentries = curenvsize + argc - 1;
      // don't forget the NULL entry at the end!
      newenviron = malloc((maxentries+1) * sizeof(char*));

      // use the function above to copy an array terminated by a NULL entry
      copynulltermarray(environ, newenviron);

      // then use another function, above, to copy a fixed number of entries
      // Note: the parameters here are a bit tricky...ask questions if confused!
      copyarray(argv + 1, newenviron + curenvsize, argc - 1);
      newenviron[maxentries] = NULL;
    }
    else
    {
      // don't forget the NULL entry at the end!
      newenviron = malloc((argc-2+1) * sizeof(char*));

      // copy everything in argv after the "-i" into the environment
      // As above: the parameters are tricky...ask questions!
      copyarray(argv + 2, newenviron, argc - 2);
      newenviron[argc - 2] = NULL;
    }

    environ = newenviron;
  }

  printenviron();

  return 0;
}
