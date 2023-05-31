#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

//variables for keeping track of inodes that have already been seen
int prev_inodes[1000];
int inode_count = 0;

//Return 1 if the inode given is new
//0 if the inode has been previouslly been seen

int is_new_inode(int inode_num)
{
  int i;
  for (i = 0; i < inode_count; i++)
    {
      if (prev_inodes[i] == inode_num)
	  return 0;
    }
  prev_inodes[inode_count] = inode_num;
  inode_count++;
  return 1;

}

int isdirectory(char *path)
{
  struct stat statbuf;
  if (lstat(path, &statbuf) == -1)
    {
      return 0;
    }
  else 
    {
      return S_ISDIR(statbuf.st_mode);
    }
}

int main(int argc, char*argv[])
{
  if (argc != 2)
    {
      fprintf(stderr, "Usage: %s directory\n", argv[0]);
      exit(EXIT_FAILURE);
    }

  char* directory = argv[1];
  //directory = argv[1];
  int isdir;
  //int to keep track of total size of files in given directory, including duplicates
  int totaldirsize = 0;

  //without duplicates
  int nodupsize = 0;

  //this variable is to try to check if the inode being seen is new
  int newcheck;


  //in the case that the path given leads to a path instead of a directory

  if ((isdir = isdirectory(directory)) == 0)
    {
      fprintf(stderr, "Error: not a directory\n");
    }
  else
    {
      
      DIR* dp;
      struct dirent* direntp;

      if ((dp = opendir(argv[1])) == NULL)
	{
	  perror("Failed to open directory");
	  exit(1);
	}

      while ((direntp = readdir(dp)) != NULL)
	{

	  char* dirstr = malloc((strlen(direntp->d_name) * sizeof(char)) + (strlen(directory) * sizeof(char)) + 1);
	  memset(dirstr, 0, ((strlen(direntp->d_name) * sizeof(char)) + (strlen(directory) * sizeof(char))) + 1);

	  strcat(dirstr, directory);
	  strcat(dirstr, "/");
	  strcat(dirstr, direntp->d_name);
	  
	  struct stat filestat;

	  if (lstat(dirstr, &filestat) == -1)
	    {
	      printf("I assume i went here\n");
	      perror("Unable to stat file");
	      exit(1);
	    }

	  totaldirsize = totaldirsize + filestat.st_size;

	  //nodupsize = nodupsize;

	  //printf("is_new_inode is returning: %d\n", is_new_inode(filestat.st_ino));
	  
	  // printf("nodupsize is: %d\n", nodupsize);

	  // printf("File being checked: %s\n", direntp->d_name);
	  // printf("inode number checking: %ld\n", filestat.st_ino);
	  // printf("what is inode_count: %d\n", inode_count);

	  if ((newcheck = is_new_inode(filestat.st_ino)) == 1)
	    {
	      //rewinddir(dp);
	      nodupsize = nodupsize + filestat.st_size;
	    }

	  

	}

      
      
      //printf("Wow! its probably a directory?\n");
      printf("Total size: %d\n", totaldirsize);
      printf("Size without duplicates: %d\n", nodupsize);

      closedir(dp);
    }

  return 0;
}
