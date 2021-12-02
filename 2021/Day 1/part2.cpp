#include <cstdio>
#include <cstdlib>
#include <queue>

const int MAX_LINE = 11;

int
main()
{
  int prev_sum = 0;
  int count = 0;
  char input[MAX_LINE];
  std::queue<int> depth_stack;

  // get the first three measurement sum
  fgets(input, MAX_LINE, stdin);
  prev_sum += atoi(input);
  depth_stack.push(atoi(input));
  for (int i = 0; i < 2; i++)
    {
      fgets(input, MAX_LINE, stdin);
      int curr = atoi(input);
      prev_sum += curr;
      depth_stack.push(curr);
    }

  while (fgets(input, MAX_LINE, stdin))
    {
      // move the window and get the current sum
      int curr_sum = prev_sum - depth_stack.front() + atoi(input);
      depth_stack.pop();
      depth_stack.push(atoi(input));

      // keep track of when the current sum is greater than the previous
      if (curr_sum > prev_sum)
        count++;
      prev_sum = curr_sum;
    }

  printf("%d\n", count);
  return 0;
}
