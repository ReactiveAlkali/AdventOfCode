#include <iostream>
#include <unordered_map>
#include <string>
#include <list>
#include <sstream>
#include <unordered_set>

using adjacency_list_t = std::unordered_map<std::string,std::list<std::string>>;

//--------------------------------------------------------------------------
// Parses the given input into an adjacency list which is implemented as an
// unordered map of string lists with string keys
//--------------------------------------------------------------------------
adjacency_list_t
parse_input ()
{
  adjacency_list_t cave_system {};
  std::string input_line {};

  while (std::cin >> input_line)
    {
      std::stringstream ss { input_line };
      std::string cave_1;
      std::string cave_2;

      // get the labels of the two connected caves
      std::getline(ss, cave_1, '-');
      std::getline(ss, cave_2, '-');

      // add the connected caves to our adjacency list
      cave_system[cave_1].push_back(cave_2);
      cave_system[cave_2].push_back(cave_1);
    }

  return cave_system;
}

//--------------------------------------------------------------------------
// Determines whether the given string is small.  Note that large caves are
// all uppercase while small caves are all lower case.
//--------------------------------------------------------------------------
bool
is_small_cave (std::string cave)
{
  return cave[0] >= 'a' && cave[0] <= 'z';
}

//---------------------------------------------------------------------
// Recursively counts the number of paths which leads to the end point
// Returns the number of paths from the current cave to the end
//---------------------------------------------------------------------
int
rec_path_search (adjacency_list_t& cave_system,
                 std::unordered_set<std::string>& visited,
                 const std::string& curr_cave, bool small_twice)
{
  int paths {};

  if (curr_cave == "end")
    paths = 1;
  else if (!is_small_cave(curr_cave) || visited.count(curr_cave) == 0
           || (!small_twice && curr_cave != "start"))
    {
      // keep track of whether we've visited a small cave twice
      if (is_small_cave(curr_cave) && visited.count(curr_cave) == 1)
        {
          for (const std::string& cave : cave_system[curr_cave])
            paths += rec_path_search(cave_system, visited, cave, true);
        }
      else
        {
          visited.insert(curr_cave);
      
          // continue the path search to connected caves
          for (const std::string& cave : cave_system[curr_cave])
            paths += rec_path_search(cave_system, visited, cave, small_twice);

          visited.erase(curr_cave);
        }
    }
 
  return paths;
}

//-----------------------------------------------------------------
// Finds the total number of paths from the start point to the end
//-----------------------------------------------------------------
int
path_search (adjacency_list_t& cave_system)
{
  std::unordered_set<std::string> visited {};
  return rec_path_search(cave_system, visited, "start", false);
}

int
main ()
{
  adjacency_list_t cave_system { parse_input() };
  int total_paths { path_search(cave_system) };

  std::cout << "Total Paths: " << total_paths << '\n';

  return 0;
}
