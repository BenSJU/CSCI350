#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s randomseed [maxnumber]\n", argv[0]);
    exit(1);
  }
  srand(atoi(argv[1]));

  int correctnumber;
  if (argc == 2)
    {
      correctnumber = rand() % 101;
    }
  else
    {
      correctnumber = rand() % (atoi(argv[2]) + 1);
    }

  int guessed = 0;
  do {
      printf("Guess the number: ");
      scanf("%d", &guessed);

      if (guessed < correctnumber)
	{
	  printf("Higher!\n");
	}
      else if (guessed > correctnumber)
	{
	  printf("Lower!\n");
	}
      else if (guessed == correctnumber)
	{
	  printf("You guessed it!\n");
	}
  }while(guessed != correctnumber);
    //printf("Guess the number: ");
  // scanf("%d", &guessed);
    // printf("Wrong!\n");
  return 0;
}
