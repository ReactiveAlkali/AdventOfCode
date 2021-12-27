#include <iostream>
#include <string>
#include <list>
#include <array>
#include <iterator>

// I've choosen to represent a snailfish number as a list of two element int
// arrays.  The first element being some number value held by the snailfish
// number while the second element gives the depth of that number.
using snailfish_number_t = std::list<std::array<int,2>>;

//
// Parses our puzzle input into a list of snailfish numbers
//
std::list<snailfish_number_t>
parse_input ()
{
  std::list<snailfish_number_t> sf_nums {};
  std::string input_line {};

  while (std::cin >> input_line)
    {
      snailfish_number_t curr_sf_num {};
      int curr_depth { -1 };

      for (char c : input_line)
        {
          // The only characters that should be in the puzzle input are square
          // braces, commas, and numbers 0 to 9
          switch (c)
            {
            case '[':
              ++curr_depth;
              break;
            case ']':
              --curr_depth;
              break;
            case ',':
              break;
            default:
              std::array<int,2> element { c - '0', curr_depth };
              curr_sf_num.push_back(element);
            }
        }
      
      sf_nums.push_back(curr_sf_num);
    }

  return sf_nums;
}

//
// Explodes any pair nested inside four other pairs.  Returns whether a pair
// was exploded.
//
bool
explode_four_deep_pairs (snailfish_number_t& sf_num)
{
  bool exploded_pair { false };
  snailfish_number_t::iterator it { sf_num.begin() };

  while (it != sf_num.end())
    {
      if ((*it)[1] == 4)
        {
          exploded_pair = true;
          int num { (*it)[0] };

          // Remove the first member of the pair and add its value to the first
          // value to the left if one exists.
          it = sf_num.erase(it);
          if (it != sf_num.begin())
            {
              auto tmp { it };
              --tmp;
              (*tmp)[0] += num;
            }

          // From working through things on paper this always ends up in the
          // middle for the examples I've done by hand
          sf_num.insert(it, std::array<int,2>{ 0, 3 });

          // Do basically the same thing again but going right
          num = (*it)[0];
          it = sf_num.erase(it);
          if (it != sf_num.end())
            (*it)[0] += num;
        }
      else
        ++it;
    }

  return exploded_pair;
}

//
// Splits any regular number with a value of 10 or greater.  Returns true if
// any was split.
//
bool
split_numbers (snailfish_number_t& sf_num)
{
  bool split_number { false };
  snailfish_number_t::iterator it { sf_num.begin() };

  while (it != sf_num.end() && !split_number)
    {
      if ((*it)[0] > 9)
        {
          split_number = true;

          // When we split a number we replace it with a pair of numbers a
          // depth lower which sum to the splitted number.
          std::array<int,2> e { *it };
          it = sf_num.erase(it);

          sf_num.insert(it, std::array<int,2>{ e[0] / 2, e[1] + 1 });
          sf_num.insert(it, std::array<int,2>{ e[0] / 2 + e[0] % 2, e[1] + 1 });
        }
      else
        ++it;
    }

  return split_number;
}

//
// Reduce the given snailfish number
//
void
reduce_snailfish_number (snailfish_number_t& sf_num)
{
  bool reduce_more { true };

  while (reduce_more)
    {
      bool exploded { explode_four_deep_pairs(sf_num) };
      bool split { split_numbers(sf_num) };
      reduce_more = exploded || split;
    }
}

//
// Adds two snailfish numbers together.  Adding snailfish numbers means
// creating a new pair from the left and right parameters of the operation.
//
snailfish_number_t
add_snailfish_numbers (const snailfish_number_t& sf_num1,
                       const snailfish_number_t& sf_num2)
{
  snailfish_number_t sum { sf_num1 };
  sum.insert(sum.end(), sf_num2.begin(), sf_num2.end());

  // due to the addition everything's depth increases by one
  for (auto& elem : sum)
    ++elem[1];

  reduce_snailfish_number(sum);

  return sum;
}

//
// Calculates the magnitude of the given snailfish number
//
int
snailfish_number_magnitude (const snailfish_number_t& sf_num)
{
  snailfish_number_t mag_sf { sf_num };

  // We calculate the magnitude of each number pair working our way up the
  // tree.  Each pair gets replaced by a value a depth higher containing
  // its magnitude.
  for (int curr_depth { 3 }; curr_depth >= 0; --curr_depth)
    {
      snailfish_number_t::iterator it { mag_sf.begin() };

      while (it != mag_sf.end())
        {
          if ((*it)[1] == curr_depth)
            {
              int curr_mag { 3 * (*it)[0] };
              it = mag_sf.erase(it);

              curr_mag += 2 * (*it)[0];
              it = mag_sf.erase(it);

              mag_sf.insert(it, std::array<int,2>{ curr_mag, curr_depth - 1 });
            }
          else
            ++it;
        }
    }

  return mag_sf.front()[0];
}

//
// Calculates the sum of the given list of snailfish numbers and returns the
// magnitude of the calculated sum.
//
int
sum_magnitude (const std::list<snailfish_number_t>& sf_nums)
{
  snailfish_number_t sum { sf_nums.front() };

  std::list<snailfish_number_t>::const_iterator it { sf_nums.begin() };
  ++it;

  while (it != sf_nums.end())
    {
      sum = add_snailfish_numbers(sum, *it);
      ++it;
    }

  return snailfish_number_magnitude(sum);
}

//
// Finds the largest magnitude that can be obtained by adding any two of the
// given snailfish numbers together.
//
int
find_max_magnitude (const std::list<snailfish_number_t>& sf_nums)
{
  int max_magnitude {};

  std::list<snailfish_number_t>::const_iterator it1 { sf_nums.begin() };
  while (it1 != sf_nums.end())
    {
      std::list<snailfish_number_t>::const_iterator it2 { it1 };
      // we increment the it2 iterator because we want to add different
      // snailfish numbers
      ++it2;

      while (it2 != sf_nums.end())
        {
          // Snailfish numbers are not commutative so we check the magnitudes of
          // both sums.
          snailfish_number_t sum1 = add_snailfish_numbers(*it1, *it2);
          snailfish_number_t sum2 = add_snailfish_numbers(*it2, *it1);

          int mag1 = snailfish_number_magnitude(sum1);
          int mag2 = snailfish_number_magnitude(sum2);

          if (mag1 > max_magnitude)
            max_magnitude = mag1;
          if (mag2 > max_magnitude)
            max_magnitude = mag2;

          ++it2;
        }
      ++it1;
    }

  return max_magnitude;
}

int
main ()
{
  std::list<snailfish_number_t> sf_nums { parse_input() };

  std::cout << "Sum magnitude: " << sum_magnitude(sf_nums) << '\n';
  std::cout << "Largest magnitude: " << find_max_magnitude(sf_nums) << '\n';

  return 0;
}
