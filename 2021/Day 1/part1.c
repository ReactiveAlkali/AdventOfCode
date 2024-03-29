#include <stdio.h>
#include <stdlib.h>

const int MAX_LINE = 11;

int
main()
{
  int prev_depth;
  int count = 0;
  char input[MAX_LINE];

  fgets(input, MAX_LINE, stdin);
  prev_depth = atoi(input);

  while (fgets(input, MAX_LINE, stdin))
    {
      int curr_depth = atoi(input);
      if (curr_depth > prev_depth)
        count++;
      prev_depth = curr_depth;
    }

  printf("%d\n", count);
  return 0;
}
