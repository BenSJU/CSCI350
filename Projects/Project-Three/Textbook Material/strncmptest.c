/* strncmp example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char str[][12] = { "export this" , "C3PO" , "export that", "export aaaa", "R2abcd" };
  int n;
  puts ("Looking for R2 astromech droids...");
  for (n=0 ; n<5 ; n++)
    if (strncmp (str[n],"export",6) == 0)
    {
      printf ("found %s\n",str[n]);
    }
  return 0;
}
