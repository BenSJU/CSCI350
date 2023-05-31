#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int makeargv(const char *s, const char *delimiters, char ***argvp) {
   int error;
   int i;
   int numtokens;
   const char *snew;
   char *t;

   if ((s == NULL) || (delimiters == NULL) || (argvp == NULL)) {
      errno = EINVAL;
      return -1;
   }
   *argvp = NULL;                           
   snew = s + strspn(s, delimiters);         /* snew is real start of string */
   if ((t = malloc(strlen(snew) + 1)) == NULL) 
      return -1; 
   strcpy(t, snew);               
   numtokens = 0;
   if (strtok(t, delimiters) != NULL)     /* count the number of tokens in s */
      for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++) ; 

                             /* create argument array for ptrs to the tokens */
   if ((*argvp = malloc((numtokens + 1)*sizeof(char *))) == NULL) {
      error = errno;
      free(t);
      errno = error;
      return -1; 
   } 
                        /* insert pointers to tokens into the argument array */
   if (numtokens == 0) 
      free(t);
   else {
      strcpy(t, snew);
      **argvp = strtok(t, delimiters);
      for (i = 1; i < numtokens; i++)
          *((*argvp) + i) = strtok(NULL, delimiters);
    } 
    *((*argvp) + numtokens) = NULL;             /* put in final NULL pointer */
    return numtokens;
} 

int main(int argc, char* argv[])
{
  
  if (argc != 3)
    {
      fprintf(stderr, "Usage: ./ocap cmd outfile\n");
      exit(EXIT_FAILURE);
    }

  //open file
  int fd;
  while ((fd = open(argv[2], CREATE_FLAGS, CREATE_MODE)) == -1 && errno == EINTR)
    ;
  if (fd == -1)
    {
      fprintf(stderr, "Failed to open or create file '%s'\n", argv[2]);
      perror(NULL);
      exit(EXIT_FAILURE);
    }

  if (dup2(fd, STDOUT_FILENO) == -1)
    {
      perror("Failed to redirect standard output");
      exit(EXIT_FAILURE);
    }

  //fork for child process
  pid_t pid = fork();

  if (pid == -1)
    {
      perror("Failed to fork child process");
      exit(1);
    }
  if(pid == 0)
    {
      char** myargv;
      int argvsuccess = makeargv(argv[1], " ", &myargv);
      if (argvsuccess == -1)
	{
	  perror("Failed to make myargv");
	  exit(EXIT_FAILURE);
	}
      // printf("Executing child process\n");
      execvp(myargv[0], &myargv[0]);
      //perror("Failed to execute child process");
      exit(EXIT_FAILURE);
    }

  wait(NULL);
  // printf("Successfully returned to parent process\n");

  //close file
  int closeresult;
  while ((closeresult = close(fd)) == -1 && errno == EINTR)
    ;
  if (closeresult == -1)
    {
      fprintf(stderr, "Failed to close file '%s'\n", argv[2]);
      perror(NULL);
      exit(EXIT_FAILURE);
    }

  return 0;

}
