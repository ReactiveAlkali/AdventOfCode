#include "IntVector.h"
#include <stdio.h>
#include <stdlib.h>

int
main ()
{
  IntVector* expense_report = IntVector_new();

  if (expense_report == NULL)
    exit(EXIT_FAILURE);

  // read in our input
  int input;
  while (scanf("%d", &input) != EOF)
    IntVector_push_back(expense_report, input);

  int result = -1;
  for (size_t i = 0; i < IntVector_size(expense_report) && result < 0; i++)
    {
      int num1 = *IntVector_at(expense_report, i);
      for (size_t j = i + 1; j < IntVector_size(expense_report) && result < 0; j++)
        {
          int num2 = *IntVector_at(expense_report, j);
          if (num1 + num2 == 2020)
            result = num1 * num2;
        }
    }

  printf("%d\n", result);

  return 0;
}
