#include <iostream>
#include <vector>
#include <string>
#include <utility>

std::vector<std::vector<char>>
parse_input ()
{
  std::vector<std::vector<char>> sea_cucumbers {};
  std::string input_line {};

  while (std::cin >> input_line)
    {
      std::vector<char> v(input_line.begin(), input_line.end());
      sea_cucumbers.push_back(v);
    }

  return sea_cucumbers;
}

bool
step (std::vector<std::vector<char>>& sea_cucumbers)
{
  bool moved { false };
  std::vector<std::vector<int>> to_move {};
  
  // check the east facing herd
  for (int i {}; i < sea_cucumbers.size(); ++i)
    {
      for (int j {}; j < sea_cucumbers[i].size(); ++j)
        {
          if (sea_cucumbers[i][j] == '>' && ((j < sea_cucumbers[i].size() - 1
                                              && sea_cucumbers[i][j + 1] == '.')
                                             || (j == sea_cucumbers[i].size() - 1
                                                 && sea_cucumbers[i][0] == '.')))
            {
              to_move.push_back(std::vector<int>{i,j});
              moved = true;
            }
        }
    }
  // move the east facing herd
  for (auto& pos : to_move)
    {
      if (pos[1] != sea_cucumbers[pos[0]].size() - 1)
        std::swap(sea_cucumbers[pos[0]][pos[1]], sea_cucumbers[pos[0]][pos[1]+1]);
      else
        std::swap(sea_cucumbers[pos[0]][pos[1]], sea_cucumbers[pos[0]][0]);
    }
  to_move.clear();
  
  // check the south facing herd
  for (int i {}; i < sea_cucumbers.size(); ++i)
    {
      for (int j {}; j < sea_cucumbers[i].size(); ++j)
        {
          if (sea_cucumbers[i][j] == 'v' && ((i < sea_cucumbers.size()-1
                                              && sea_cucumbers[i+1][j] == '.')
                                             || (i == sea_cucumbers.size()-1
                                                 && sea_cucumbers[0][j] == '.')))
            {
              to_move.push_back(std::vector<int>{i,j});
              moved = true;
            }
        }
    }
  // move the south facing herd
  for (auto& pos : to_move)
    {
      if (pos[0] != sea_cucumbers.size() - 1)
        std::swap(sea_cucumbers[pos[0]][pos[1]], sea_cucumbers[pos[0]+1][pos[1]]);
      else
        std::swap(sea_cucumbers[pos[0]][pos[1]], sea_cucumbers[0][pos[1]]);
    }

  return moved;
}

int
main ()
{
  std::vector<std::vector<char>> sea_cucumbers { parse_input() };
  
  int steps { 1 };
  while (step(sea_cucumbers))
    ++steps;

  std::cout << steps << '\n';
  
  return 0;
}
