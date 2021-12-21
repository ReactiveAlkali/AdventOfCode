#include <iostream>
#include <array>
#include <string>

std::array<std::array<int,10>,10>
parse_input ()
{
  std::array<std::array<int,10>,10> octopi {};
  std::string input_line {};
  int index {};

  while (std::cin >> input_line)
    {
      for (int i = 0; i < input_line.size(); ++i)
        {
          octopi[index][i] = input_line[i] - 48;
        }
      ++index;
    }

  return octopi;
}

int
flash (std::array<std::array<int,10>,10>& octopi, std::size_t row, std::size_t col)
{
  int flashes { 1 };
  
  // set the starting row and column for neighbour checking
  std::size_t start_row { (row == 0) ? 0 : row - 1 };
  std::size_t start_col { (col == 0) ? 0 : col - 1 };
  std::size_t end_row { (row == 9) ? 9 : row + 1 };
  std::size_t end_col { (col == 9) ? 9 : col + 1};

  // the given octopus flashed so it has zero energy
  octopi[row][col] = 0;

  // increment the energy of neighbours
  for (std::size_t i { start_row }; i <= end_row; ++i)
    {
      for (std::size_t j { start_col }; j <= end_col; ++j)
        {
          // don't increment if we've already flashed
          if (octopi[i][j] > 0)
            {
              ++octopi[i][j];
              // flash the neighbour if it goes over 9
              if (octopi[i][j] > 9)
                flashes += flash(octopi, i, j);
            }
        }
    }

  return flashes;
}

void print_array(std::array<std::array<int,10>,10>& a);

//-----------------------------------------------
// Performs a step of the octopus simulation
// Returns how many flashes occured in that step
//-----------------------------------------------
int
step (std::array<std::array<int,10>,10>& octopi)
{
  int flashes {};
  
  // increase the energy level of each octopus by 1
  for (std::array<int,10>& row : octopi)
    {
      for (int& octopus : row)
        ++octopus;
    }

  // flash any octopus with an energy level over 9
  for (std::size_t i {}; i < 10; ++i)
    {
      for (std::size_t j {}; j < 10; ++j)
        {
          if (octopi[i][j] > 9)
            flashes += flash(octopi, i, j);
        }
    }

  return flashes;
}

int
main ()
{
  std::array<std::array<int,10>,10> octopi { parse_input() };

  // simulate 100 steps
  int flashes {};
  for (int i {}; i < 100; ++i)
    flashes += step(octopi);
  std::cout << "Flashes after 100 steps: " << flashes << '\n';

  // continue until all octopi flash simultaneously
  int steps { 101 };
  while (step(octopi) != 100)
    ++steps;
  std::cout << "Steps to syncronize: " << steps << '\n';
  
  return 0;
}
