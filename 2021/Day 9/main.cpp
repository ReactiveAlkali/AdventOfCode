#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <functional>

//---------------------------------------------
// Parses the heightmap input into a 2D vector
//---------------------------------------------
std::vector<std::vector<int>>
parse_input ()
{
  std::vector<std::vector<int>> heightmap {};
  std::string input_str {};

  while (std::cin >> input_str)
    {
      std::vector<int> input_line {};
      for (char c : input_str)
        input_line.push_back(c - '0');
      heightmap.push_back(input_line);
    }

  return heightmap;
}

//------------------------------------------------------------------------
// Determines whether the given location in the heightmap is a low point
// Low points being points that are lower than all its adjacent locations
//------------------------------------------------------------------------
bool
is_low_point (const std::vector<std::vector<int>>& heightmap, int i, int j)
{
  bool low_point { true };

  // check the neighbouring points
  if (low_point && i > 0)
    low_point = heightmap[i][j] < heightmap[i-1][j];
  if (low_point && i < heightmap.size() - 1)
    low_point = heightmap[i][j] < heightmap[i+1][j];
  if (low_point && j > 0)
    low_point = heightmap[i][j] < heightmap[i][j-1];
  if (low_point && j < heightmap[i].size() - 1)
    low_point = heightmap[i][j] < heightmap[i][j+1];

  return low_point;
}

//---------------------------------------------------------------
// Finds all the low points in the given heightmap
// Returns a 2d array containing the locations of each low point
//---------------------------------------------------------------
std::vector<std::pair<int,int>>
find_low_points (const std::vector<std::vector<int>>& heightmap)
{
  std::vector<std::pair<int,int>> low_points {};

  for (int i = 0; i < heightmap.size(); ++i)
    {
      for (int j = 0; j< heightmap[i].size(); ++j)
        {
          if (is_low_point(heightmap, i, j))
            low_points.push_back(std::make_pair(i, j));
        }
    }

  return low_points;
}

//---------------------------------------------------------------------------
// Recurses through a basin counting the number of points it is comprised of
//---------------------------------------------------------------------------
int
rec_basin_size (const std::vector<std::vector<int>>& heightmap,
                std::vector<std::vector<bool>>& visited, int curr_row,
                int curr_col)
{
  int size {};

  if (curr_row >= 0 && curr_row < heightmap.size() && curr_col >= 0
      && curr_col < heightmap[curr_row].size() && !visited[curr_row][curr_col]
      && heightmap[curr_row][curr_col] != 9)
    {
      visited[curr_row][curr_col] = true;
      ++size;

      // recurse over to adjacent cells
      size += rec_basin_size(heightmap, visited, curr_row - 1, curr_col);
      size += rec_basin_size(heightmap, visited, curr_row + 1, curr_col);
      size += rec_basin_size(heightmap, visited, curr_row, curr_col - 1);
      size += rec_basin_size(heightmap, visited, curr_row, curr_col + 1);
    }

  return size;
}

//-----------------------------------------------------------------
// Calculates the size of the basin located at the given low point
//-----------------------------------------------------------------
int
basin_size (const std::vector<std::vector<int>>& heightmap, int row, int col)
{
  int size {};
  std::vector<std::vector<bool>> visited(heightmap.size(),
                                         std::vector<bool>(heightmap[0].size(), false));
  size = rec_basin_size(heightmap, visited, row, col);

  return size;
}

int
main ()
{
  int risk_sum {};
  std::vector<std::vector<int>> heightmap { parse_input() };

  std::vector<std::pair<int,int>> low_points { find_low_points(heightmap) };

  // calculate the size of each basin
  std::vector<int> basin_sizes {};
  for (const std::pair<int,int> low_point : low_points)
    {
      basin_sizes.push_back(basin_size(heightmap, low_point.first,
                                       low_point.second));
    }

  // find the product of the three largest basins
  std::sort(basin_sizes.begin(), basin_sizes.end(), std::greater<int>());
  int result = basin_sizes[0] * basin_sizes[1] * basin_sizes[2];

  std::cout << result << '\n';
  
  return 0;
}
