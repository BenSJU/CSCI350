#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

int isdirectory(char *path) {
   struct stat statbuf;

   if (stat(path, &statbuf) == -1)
     {
       // printf("%d\n", errno);
       // printf("%s\n", strerror(errno));
       printf("went through there\n");
       return 0;
     }
   else
     {
       printf("went through here\n");
       //printf("%d\n", S_ISDIR(statbuf.st_mode));
       return S_ISDIR(statbuf.st_mode);
     }
}

//void dash

int main(int argc, char* argv[])
{
  if (argc != 2 && argc != 3)
    {
      fprintf(stderr, "Usage: %s [-l] path\n", argv[0]);
      exit(EXIT_FAILURE);
    }

  char* directory;

  if (strcmp(argv[1], "-l") == 0)
    {
      directory = argv[2];
    }
  else
    {
      directory = argv[1];
    }

  int isdir;

  // isdir = isdirectory(directory);
  // printf("%d\n", isdir);

  if ((isdir = isdirectory(directory)) == 0)
    {
      if (errno != 0)
	{
	  perror("Unable to stat path");
	  exit(1);
	}
      else
	{
	  printf("Im goin here\n");
	  printf("%s", directory);

	  //printf("%d\n", strcmp(argv[1], "-l"));

	  if (strcmp(argv[1], "-l") == 0)
	    {
	    

	      char* dirstr = malloc((strlen(directory) * sizeof(char)));
	      memset(dirstr, 0, ((strlen(directory) * sizeof(char))));

	      struct stat filestat;

	      strcat(dirstr, directory);

	      
	      if (stat(dirstr, &filestat) == -1)
		{
		  perror("Unable to stat file");
		  exit(1);
		}

	      printf(" %ld", (long int)filestat.st_uid);
	      printf(" %ld", (long int)filestat.st_gid);
	      printf(" %ld", (long int)filestat.st_size);
	      
	      free(dirstr);
	      //closedir(dd);
	    }
	  //closedir(dd);
	  printf("\n");
	}
    }
  else
    {
      
      DIR* dp;
      struct dirent* direntp;

      printf("Goin through somewhere else entirely\n");

      if((dp = opendir(directory)) == NULL)
	{
	  perror("Failed to open directory");
	  exit(1);
	}

      while ((direntp = readdir(dp)) != NULL)
	{
	  
	  printf("%s", direntp->d_name);
	  //printf("%zu ", strlen(directory));
	  if (strcmp(argv[1], "-l") == 0)
	    {
	      char* dirstr = malloc((strlen(direntp->d_name) * sizeof(char)) + (strlen(directory) * sizeof(char)) + 1);
	      memset(dirstr, 0, ((strlen(direntp->d_name) * sizeof(char)) + (strlen(directory) * sizeof(char))) + 1);
	      
	      struct stat filestat;

	      strcat(dirstr, directory);
	      strcat(dirstr, "/");
	      strcat(dirstr, direntp->d_name);

	      // printf("%s\n", dirstr);

	      if (stat(dirstr, &filestat) == -1)
		{
		  perror("Unable to stat file");
		  exit(1);
		}

	      printf(" %ld", (long int)filestat.st_uid);
	      printf(" %ld", (long int)filestat.st_gid);
	      printf(" %ld", (long int)filestat.st_size);
	      
	      free(dirstr);
	      
	      // printf("%s ", statbuf.st_uid);
	    }
	  printf("\n");
	}
      
      if (errno != 0)
	{
	  perror("Error reading directory entry");
	  exit(1);
	}

      closedir(dp);
      //exit(1);

      //return 0;
    }

  return 0;
}
