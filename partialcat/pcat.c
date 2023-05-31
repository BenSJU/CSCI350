#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  //error if more or less than 4 arguments
  if (argc != 4)
    {
      fprintf(stderr, "Usage: %s file startbyte numbytes\n", argv[0]);
      exit(EXIT_FAILURE);
    }

  //char* filename = argv[1];
  int offset = atoi(argv[2]);
  int totalbytesread = 0;
  int numc = atoi(argv[3]);

  //open file and exit error if issue with opening file
  int fd;
  while ((fd = open(argv[1], O_RDONLY)) == -1 && errno == EINTR) ;
  if (fd == -1)
  {
    fprintf(stderr, "Failed to open file '%s'\n", argv[1]);
    perror(NULL);
    exit(EXIT_FAILURE);
  }

  //allocated space for number of bytes to be read, dependent on argv[3]
  char* buf = malloc((numc * sizeof(char)) + (((numc + offset) / 64) * sizeof(char)));

  //offset space for characters to be read
  //check for errors as well
  int f1;
  while((f1 = lseek(fd, offset, SEEK_SET)) == -1 && errno == EINTR)
    ;
  if (f1 == -1)
    {
      perror("Failed to successfuly offset character");
      free(buf);
      exit(EXIT_FAILURE);
    }

  while (numc > 0)
    {
      size_t bytestoread = (numc < 64 ? numc : 64);

      //read into buffer, try not to read more than 64 chars at a time
      //exit in case of really bad errors
      int bytesread;
      while ((bytesread = read(fd, buf, numc)) == -1 && errno == EINTR)
	;
      if (bytesread == -1)
	{
	  perror("Failed to read");
	  free(buf);
	  exit(EXIT_FAILURE);
	}
      else if (bytesread == 0)
	{
	  //reached end of file
	  printf("%d", (int)totalbytesread);
	  break;
	}


      numc -= bytesread;
      totalbytesread += bytesread;
    }

  
  

  printf("%d", bytesread);

  //write contents of buffer to standard output
  //handle errors
  int byteswritten;
  while ((byteswritten = write(STDOUT_FILENO, buf, bytesread)) == -1 && errno == EINTR)
     ;
  if (byteswritten == -1)
    {
      perror("Failed to write stdout... for some reason...");
      free(buf);
      exit(EXIT_FAILURE);
    }


  //free allocated memory
  free(buf);


  //close file before leaving
  int closeresult;
  while ((closeresult = close(fd)) == -1 && errno == EINTR) ;
  if (closeresult == -1)
  {
    fprintf(stderr, "Failed to close file '%s'\n", argv[1]);
    perror(NULL);
    exit(EXIT_FAILURE);
  }

  return 0;
}
