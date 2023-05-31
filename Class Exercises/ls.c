#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char* argv[])
{
  if (argc != 2)
    {
      fprintf(stderr, "Usage: %s directory\n", argv[0]);
      exit(1);
    }

  DIR* dp;
  if ((dp = opendir(argv[1])) == NULL)
    {
      perror("Failed to open directory");
      exit(1);
    }

  struct dirent* direntp;
  while ((direntp = readdir(dp)) != NULL)
    {
      printf("%s\n", direntp->d_name);
    }
  if (errno != 0)
    {
      perror("Error reading directory entry");
      exit(1);
    }

  closedir(dp);

  return 0;
}
