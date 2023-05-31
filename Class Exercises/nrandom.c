#include <stdio.h>

int main()
{
  int n;

  srand(100);

  printf("Enter how many numbers you want: ");
  scanf("%d", &n);

  int i;
  for (i = 0; i < n; i++)
  {
    int rnum = rand() % 101;
    printf("%d\n", rnum);
  }

  return 0;
}
