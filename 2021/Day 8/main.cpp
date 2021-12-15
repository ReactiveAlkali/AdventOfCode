#include <iostream>
#include <string>
#include <tuple>
#include <array>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

//-----------------------------------------------------------------------------
// Parses a line of input from stdin.
//
// Parsed input is returned as a tuple of two string arrays.  The first array
// contains the signal patterns for that line while the second contains the
// output value.  Note that the array of signal patterns is sorted in ascending
// order by set size.
//-----------------------------------------------------------------------------
std::tuple<std::array<std::set<char>, 10>, std::array<std::set<char>, 4>>
parse_line ()
{
  std::array<std::set<char>, 10> signal_patterns{};
  std::array<std::set<char>, 4> output_value{};
  std::string input;
  std::stringstream str_strm;

  // extract the signal patterns
  std::getline(std::cin, input, '|');
  str_strm = std::stringstream(input);
  for (int i = 0; i < 10; ++i)
    {
      std::string pattern;
      str_strm >> pattern;
      signal_patterns[i] = std::set<char>(pattern.begin(), pattern.end());
    }

  // Sort the signal patterns by size.  Note that because 1, 4, 7, and 8 all
  // have unique lengths they will always be located at the same indexes of the
  // array.
  std::sort(signal_patterns.begin(), signal_patterns.end(),
            [](const std::set<char>& l, const std::set<char>& r)
            {
              return l.size()<r.size();
            });

  // extract the output value
  std::getline(std::cin, input);
  str_strm = std::stringstream(input);
  for (int i = 0; i < 4; ++i)
    {
      std::string value;
      str_strm >> value;
      output_value[i] = std::set<char>(value.begin(), value.end());
    }

  std::cin >> std::ws;

  return std::make_tuple(signal_patterns, output_value);
}

//--------------------------------------------------
// Maps a set of signals to their respective digits
//--------------------------------------------------
std::map<std::set<char>, std::string>
map_signals_to_digits (const std::array<std::set<char>, 10>& signal_patterns)
{
  std::map<std::set<char>, std::string> mapping {};

  // Map digits known by their number of segments
  mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[0], "1"));
  mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[1], "7"));
  mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[2], "4"));
  mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[9], "8"));

  // Get the differnce between segments used by 1 and 4
  std::set<char> four_diff {};
  std::set_difference(signal_patterns[2].begin(), signal_patterns[2].end(),
                      signal_patterns[0].begin(), signal_patterns[0].end(),
                      std::inserter(four_diff, four_diff.begin()));

  // Map patterns of length 5, indices 3 to 5
  for (int i = 3; i < 6; ++i)
    {
      if (std::includes(signal_patterns[i].begin(), signal_patterns[i].end(),
                        signal_patterns[0].begin(), signal_patterns[0].end()))
        mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[i], "3"));
      else if (std::includes(signal_patterns[i].begin(), signal_patterns[i].end(),
                             four_diff.begin(), four_diff.end()))
        mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[i], "5"));
      else
        mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[i], "2"));
    }

  // Map patterns of length 6, indices 6 to 8
  for (int i = 6; i < 9; ++i)
    {
      if (std::includes(signal_patterns[i].begin(), signal_patterns[i].end(),
                        signal_patterns[2].begin(), signal_patterns[2].end()))
        mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[i], "9"));
      else if (std::includes(signal_patterns[i].begin(), signal_patterns[i].end(),
                             signal_patterns[0].begin(), signal_patterns[0].end()))
        mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[i], "0"));
      else
        mapping.insert(std::pair<std::set<char>, std::string>(signal_patterns[i], "6"));
    }

  return mapping;
}

int
main ()
{
  int output_sum{};
  
  while (std::cin.good())
    {
      std::array<std::set<char>, 10> signal_patterns;
      std::array<std::set<char>, 4> output_value;
      std::tie(signal_patterns, output_value) = parse_line();

      std::map<std::set<char>, std::string> mapping { map_signals_to_digits(signal_patterns) };

      std::string num {};
      for (const std::set<char>& digit : output_value)
        {
          num.append(mapping[digit]);
        }
      output_sum += std::stoi(num);
    }

  std::cout << output_sum << '\n';
  return 0;
}
