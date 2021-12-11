#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

std::vector<int>
parse_input ()
{
  std::vector<int> input{};
  int num;

  while (std::cin >> num)
    {
      if (std::cin.peek() == ',')
        std::cin.ignore();
      input.push_back(num);
    }
  std::sort(input.begin(), input.end());

  return input;
}

int
calc_median (const std::vector<int>& data)
{
  int median;
  if (data.size() % 2 == 0)
    median = (data[data.size() / 2] + data[data.size() / 2 + 1]) / 2;
  else
    median = data[data.size() / 2];
  return median;
}

int
const_rate_fuel (const std::vector<int>& data, int point)
{
  int fuel{};
  for (int num : data)
    {
      fuel += std::abs(num - point);
    }
  return fuel;
}

int
calc_mean (const std::vector<int>& data)
{
  int sum{};
  for (int num : data)
    sum += num;
  return sum / data.size();
}

int
calc_fuel_consumption (const std::vector<int>& data, int point)
{
  int fuel{};
  for (int num : data)
    fuel += ((point - num) * (point - num) + std::abs(point - num)) / 2;
  return fuel;
}

int
minimum_fuel (const std::vector<int>& data)
{
  int mean { calc_mean(data) };
  int min_fuel { calc_fuel_consumption(data, mean - 1) };

  for (int i = 0; i < 2; ++i)
    {
      int fuel { calc_fuel_consumption(data, mean + i) };
      if (fuel < min_fuel)
        min_fuel = fuel;
    }

  return min_fuel;
}

int
main ()
{
  std::vector<int> input{ parse_input() };

  int median{ calc_median(input) };
  int fuel{ const_rate_fuel(input, median) };
  std::cout << "Fuel spent part 1: " << fuel << '\n';

  fuel = minimum_fuel(input);
  std::cout << "Minimum fuel spent part 2: " << fuel << '\n';
  
  return 0;
}
