#include <array>
#include <iostream>
#include <string>

std::array<long, 9>
parse_input ()
{
  std::array<long, 9> histogram{};
  std::string input;
  
  while (std::getline(std::cin, input, ','))
    {
      int days{ std::stoi(input) };
      histogram[days]++;
    }
  return histogram;
}

long
simulate (std::array<long, 9> histogram, int days)
{
  for (int i = 0; i < days; ++i)
    {
      std::array<long, 9> temp{};

      // fish reproduce and reset to 6
      temp[6] += histogram[0];
      temp[8] += histogram[0];

      for (int j = 0; j < 8; ++j)
        {
          temp[j] += histogram[j+1];
        }
      histogram = temp;
    }

  // sum the total amount of fish
  long total_fish = 0;
  for (long num : histogram)
    total_fish += num;

  return total_fish;
}

int
main ()
{
  std::array<long, 9> input{ parse_input() };

  std::cout << "Total fish after 80 days: " << simulate(input, 80) << '\n';
  std::cout << "Total fish after 256 days: " << simulate(input, 256) << '\n';

  return 0;
}
