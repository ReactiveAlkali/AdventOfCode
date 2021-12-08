#include <unordered_map>
#include <list>
#include <iostream>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <functional>

// extremely simple way of hashing a std::pair
struct pair_hash
{
  template <class T1, class T2>
  std::size_t
  operator () (const std::pair<T1, T2> &p) const
  {
    auto h1 = std::hash<T1>{}(p.first);
    auto h2 = std::hash<T2>{}(p.second);
    return h1 ^ h2;
  }
};

using vertex_t = std::pair<int, int>;
using line_t = std::pair<vertex_t, vertex_t>;

std::unordered_map<vertex_t, int, pair_hash> vertex_map;
std::list<line_t> line_segments;

void
parse_input ()
{
  line_t temp;

  while (std::scanf("%d,%d -> %d,%d", &temp.first.first, &temp.first.second, &temp.second.first,
                    &temp.second.second) != EOF)
    {
      line_segments.push_back(temp);
    }
}

void
plot_line (line_t& line)
{
  int x = line.first.first;
  int y = line.first.second;
  vertex_map[std::make_pair(x, y)]++;
  while (x != line.second.first || y != line.second.second)
    {
      // increment the x and y values
      if (line.first.first != line.second.first)
        x += (line.first.first < line.second.first) ? 1 : -1;
      if (line.first.second != line.second.second)
        y += (line.first.second < line.second.second) ? 1 : -1;

      vertex_map[std::make_pair(x, y)]++;
    }
}

bool
not_diagonal (line_t& line)
{
  return (line.first.first == line.second.first) || (line.first.second == line.second.second);
}

// counts how many points have more that two lines overlap
int
count_overlap ()
{
  int count = 0;
  for (auto& element : vertex_map)
    {
      if (element.second >= 2)
        ++count;
    }
  return count;
}

void
solve_part1 ()
{
  // plot all non-diagonal lines onto the grid
  for (line_t& line : line_segments)
    {
      if (not_diagonal(line))
        plot_line(line);
    }
  std::cout << count_overlap() << '\n';
}

void
solve_part2 ()
{
  // plot all lines onto the grid
  for (line_t& line : line_segments)
    plot_line(line);
  
  std::cout << count_overlap() << '\n';
}

int
main ()
{
  parse_input();
  //solve_part1();
  solve_part2();
  return 0;
}
