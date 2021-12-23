#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <limits>
#include <sstream>

using rules_t = std::map<std::string,char>;
using char_count_t = std::map<char,std::size_t>;
using pair_count_t = std::map<std::string,std::size_t>;

//------------------------------------------------------------------
// Parses the input into three data structures returned as a tuple.
// First a count of how often each character occurs.
// Second a count of how often each character pair occurs.
// Third the rules which govern pair insertion.
//------------------------------------------------------------------
std::tuple<char_count_t,pair_count_t,rules_t>
parse_input ()
{
  std::string polymer_template {};
  rules_t insertion_rules {};
  char_count_t char_counts {};
  pair_count_t pair_counts {};

  // parses the polymer template
  std::cin >> polymer_template;
  for (std::size_t i {}; i < polymer_template.size() - 1; ++i)
    {
      ++char_counts[polymer_template[i]];
      ++pair_counts[polymer_template.substr(i, 2)];
    }
  ++char_counts[polymer_template[polymer_template.size() - 1]];

  // read in all the insertion rules
  std::string pair, temp;
  char insertion;
  while (std::cin >> pair >> temp >> insertion)
    insertion_rules[pair] = insertion;

  return std::make_tuple(char_counts, pair_counts, insertion_rules);
}

//-----------------------------------------------------------------------
// Performs a single step of pair insertion according to the given rules
//-----------------------------------------------------------------------
void
pair_insertion_step (char_count_t& char_counts, pair_count_t& pair_counts,
                     const rules_t& rules)
{
  // store the initial state
  pair_count_t pair_copy { pair_counts };

  for (const auto& pair_count : pair_copy)
    {
      std::string pair { pair_count.first };
      
      // insert a character if a rule exists for the current pair
      if (pair_count.second > 0 && rules.count(pair) == 1)
        {
          char to_insert { rules.at(pair) };
          char_counts[to_insert] += pair_count.second;

          // decrement the amount of the current pair
          pair_counts[pair] -= pair_count.second;

          // increase the count of the resultant pairs
          pair.insert(1, 1, to_insert);
          pair_counts[pair.substr(0, 2)] += pair_count.second;
          pair_counts[pair.substr(1, 2)] += pair_count.second;
        }
    }
}

//----------------------------------------------------------------------------
// Finds the quantities which the most and least common characters occur in.
// Returns a tuple where the first element corresponds to the most common and
// the second element to the least common character.
//----------------------------------------------------------------------------
std::tuple<std::size_t,std::size_t>
most_and_least_common_quantities (const char_count_t& char_counts)
{
  std::size_t most_common_quantity { std::numeric_limits<std::size_t>::min() };
  std::size_t least_common_quantity { std::numeric_limits<std::size_t>::max() };

  // find the quantities that the least and most common elements occur in
  for (const auto& char_count : char_counts)
    {
      if (char_count.second > most_common_quantity)
        most_common_quantity = char_count.second;
      if (char_count.second < least_common_quantity)
        least_common_quantity = char_count.second;
    }

  return std::make_tuple(most_common_quantity, least_common_quantity);
}

int
main (int argc, char** argv)
{
  char_count_t char_counts {};
  pair_count_t pair_counts {};
  rules_t rules {};

  // get command line input of how many steps to perform
  if (argc <= 1)
    {
      std::cout << "Usage: ./<program name> <number>\n";
      return 1;
    }

  // convert the command line input to an integer
  std::stringstream convert { argv[1] };
  int steps {};
  if (!(convert >> steps))
    steps = 0;

  std::tie(char_counts, pair_counts, rules) = parse_input();

  // perform pair insertion steps
  for (int i {}; i < steps; ++i)
    pair_insertion_step(char_counts, pair_counts, rules);

  std::size_t most {};
  std::size_t least {};
  std::tie(most, least) = most_and_least_common_quantities(char_counts);

  std::cout << most - least << '\n';
  
  return 0;
}
