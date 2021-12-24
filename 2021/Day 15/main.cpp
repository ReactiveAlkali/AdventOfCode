#include <vector>
#include <iostream>
#include <string>
#include <limits>
#include <queue>
#include <functional>

//-----------------------------------------------------------------------
// Stores information on a specific vertex including the total amount of
// risk to reach and the predecessor vertex before it.
//-----------------------------------------------------------------------
struct vertex_t
{
  int row {};
  int col {};
  int total_risk { std::numeric_limits<int>::max() };

  // define a function for comparing structs of this type
  bool
  operator> (const vertex_t& rhs) const
  {
    return total_risk > rhs.total_risk;
  }
};

using vector2D = std::vector<std::vector<int>>;
using priority_queue_t =
  std::priority_queue<vertex_t, std::vector<vertex_t>, std::greater<vertex_t>>;

//--------------------------------------------------------------------
// Tile the given risk map downwards by the specified amount of times
//--------------------------------------------------------------------
void
tile_down (vector2D& risk_map, int tiles)
{
  std::size_t original_size = risk_map.size();

  for (int i = 1; i < tiles; ++i)
    {
      for (std::size_t row {}; row < original_size; ++row)
        {
          // construct a new row based on the current row
          std::vector<int> new_row {};
          
          for (int num : risk_map[row])
            {
              int new_num { num + i };
              if (new_num > 9)
                new_num -= 9;
              new_row.push_back(new_num);
            }
          risk_map.push_back(new_row);
        }
    }
}


void
tile_right (vector2D& risk_map, int tiles)
{
  for (auto& row : risk_map)
    {
      std::size_t row_size = row.size();

      for (int i = 1; i < tiles; ++i)
        {
          for (std::size_t index{}; index < row_size; ++index)
            {
              int new_num { row[index] + i };
              if (new_num > 9)
                new_num -= 9;
              row.push_back(new_num);
            }
        }
    }
}

//-------------------------------------------
// Parses the input into a 2D integer vector
//-------------------------------------------
vector2D
parse_input ()
{
  vector2D risk_map {};
  std::string input_line {};

  while (std::cin >> input_line)
    {
      std::vector<int> new_row {};

      for (char c : input_line)
        new_row.push_back(c - '0');

      risk_map.push_back(new_row);
    }

  // tile the risk map to get a map 5 times the size
  tile_down(risk_map, 5);
  tile_right(risk_map, 5);

  return risk_map;
}

//---------------------------------------------------------------------
// Adds the neighbours of the given vertex to the given priority queue
//---------------------------------------------------------------------
void
add_neighbours (const vector2D& risk_map, const vector2D& risk,
                priority_queue_t& pq, vertex_t& v)
{
  if (v.row > 0 && risk[v.row-1][v.col] == std::numeric_limits<int>::max())
    {
      pq.push(vertex_t { v.row - 1, v.col, v.total_risk
                        + risk_map[v.row - 1][v.col] });
    }
  if (v.row < risk_map.size() - 1
      && risk[v.row+1][v.col] == std::numeric_limits<int>::max())
    {
      pq.push(vertex_t { v.row + 1, v.col, v.total_risk
                        + risk_map[v.row + 1][v.col] });
    }
  if (v.col > 0 && risk[v.row][v.col-1] == std::numeric_limits<int>::max())
    {
      pq.push(vertex_t { v.row, v.col - 1, v.total_risk
                      + risk_map[v.row][v.col - 1] });
    }
  if (v.col < risk_map[0].size() - 1
      && risk[v.row][v.col+1] == std::numeric_limits<int>::max())
    {
      pq.push(vertex_t { v.row, v.col + 1, v.total_risk
                        + risk_map[v.row][v.col + 1] });
    }
}

//------------------------------------------------------------------------
// Finds the lowest possible risk to reach the bottom right corner of the
// given cave risk map.
//------------------------------------------------------------------------
int
lowest_risk_path (const vector2D& risk_map)
{
  vector2D risk(risk_map.size(),
                std::vector<int>(risk_map[0].size(),
                                 std::numeric_limits<int>::max()));
  priority_queue_t pq {};

  // set up our source vertex
  pq.push(vertex_t{0, 0, 0});

  // loop until we find a path to the destination
  while (risk[risk.size() - 1][risk[0].size() - 1]
         == std::numeric_limits<int>::max())
    {
      // extract the vertex with the lowest risk
      vertex_t curr_v = pq.top();
      pq.pop();

      // ignore this vertex if we already visited it
      if (risk[curr_v.row][curr_v.col] == std::numeric_limits<int>::max())
        {
          risk[curr_v.row][curr_v.col] = curr_v.total_risk;
          add_neighbours(risk_map, risk, pq, curr_v);
        }
    }

  return risk[risk.size() - 1][risk[0].size() - 1];
}

int
main ()
{
  std::vector<std::vector<int>> risk_map { parse_input() };

  int risk { lowest_risk_path(risk_map) };

  std::cout << risk << '\n';
  return 0;
}
