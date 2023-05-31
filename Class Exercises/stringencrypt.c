#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
  if (argc != 2)
    {
      fprintf(stderr, "Usage: %s message\n", argv[0]);
      exit(1);
    }

  char* message = argv[1];
  int messagelength = strlen(message);

  char* encryptedmessage = malloc(messagelength * sizeof(char));

  int i;
  for (i = 0; i < messagelength; i++)
    {
      encryptedmessage[i] = message[i] + 1;
    }

  printf("%s\n", encryptedmessage);

  return 0;
}
