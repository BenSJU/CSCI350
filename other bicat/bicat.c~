#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#define CREATE_FLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int output_mode = 1;

static void sig_handler(int signo)
{
  if (signo == SIGUSR1)
    {
      //write(STDIN_FILENO, "Switching output to file", 24);
      output_mode = 0;
      // printf("%d", output_mode);
    }
}

int main(int argc, char* argv[])
{

  struct sigaction newaction;
  //struct sigaction oldaction;
  newaction.sa_handler = sig_handler;

  if (sigaction(SIGUSR1, &newaction, NULL) == -1)
    {
      perror("Failed to set signal actions");
      exit(1);
    }
  else if (sigaction(EINTR, &newaction, NULL) == -1)
    {
      perror("Failed to set signal actions");
      exit(1);
    }


  int bytesread;
  int byteswritten;
  //int sig_to_run = 1;
  char one[1];

  if (argc != 2)
    {
      fprintf(stderr, "Usage: %s outfile\n", argv[0]);
      exit(1);
    }

  int fd;
  while ((fd = open(argv[1], CREATE_FLAGS, CREATE_MODE)) == -1 && errno == EINTR)
    ;
  if (fd == -1)
    {
      perror("Failed to open or create file");
      exit(EXIT_FAILURE);
    }


  while(1)
    {

      while ((bytesread = read(STDIN_FILENO, one, 1)) == -1 && errno == EINTR)
	;
      if (bytesread == -1)
     	{
	  perror("Failed to read line");
	  exit(1);
      	}
      else if (bytesread == 0)
	{
	  //sig_to_run = 0;
	  break;
	}

      if (output_mode == 0)
	{
	  byteswritten = write(fd, one, 1);
	  if (byteswritten == -1)
	    {
	      perror("Failed to write");
	      exit(EXIT_FAILURE);
	    }
	}
      else if (output_mode == 1)
	{
	  byteswritten = write(STDOUT_FILENO, one, 1);
	  if (byteswritten == -1)
	    {
	      perror("Failed to write");
	      exit(EXIT_FAILURE);
	    }
	}
      
    }



  //printf("trying to close outfile");
  int closeresult;
  while ((closeresult = close(fd)) == -1 && errno == EINTR)
    ;
  if (closeresult == -1)
    {
      perror("Failed to close file");
      exit(1);
    }

  return 0;
}
