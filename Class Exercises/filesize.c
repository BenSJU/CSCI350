#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>



int main(int argv, char** argv)
{
  if (argv != 2)
    {
      fprintf(stderr, "Usage: %s file\n", argv[0]);
      exit(1);
    }

  struct stat filestat;

  if (stat(argv[1], &filestat) == -1)
    {
      perror("Unable to stat file");
      exit(1);
    }

  printf("File %s inode: %ld\n", argv[1], filestat.st_ino);
  printf("File %s size: %ld\n", argv[1], filestat.st_size);

  return 0;
}
