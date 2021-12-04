#include <iostream>
#include <bitset>
#include <array>
#include <list>
#include <iterator>

class BinaryList
{
  std::array<int, 12> set_count{};
  std::list<std::bitset<12>> binary_list{};

public:

  using iterator = std::list<std::bitset<12>>::iterator;
  
  void
  insert (std::bitset<12> val)
  {
    binary_list.push_back(val);
    for (int i = 0; i < 12; ++i)
      {
        if (val.test(i))
          ++set_count[i];
      }
  }

  std::bitset<12>
  front ()
  {
    return binary_list.front();
  }

  // gamma rate is the most common bit at each position
  std::bitset<12>
  calc_gamma ()
  {
    std::bitset<12> gamma;
    for (int i = 0; i < 12; ++i)
      gamma.set(i, set_count[i] >= binary_list.size() - set_count[i]);
    return gamma;
  }

  // the epsilon rate is the least common bit at each postion
  std::bitset<12>
  calc_epsilon ()
  {
    return calc_gamma().flip();
  }

  iterator
  begin ()
  {
    return binary_list.begin();
  }

  iterator
  end ()
  {
    return binary_list.end();
  }

  iterator
  erase (iterator pos)
  {
    for (int i = 0; i < 12; ++i)
      {
        if ((*pos).test(i))
          --set_count[i];
      }
    return binary_list.erase(pos);
  }

  std::size_t
  size ()
  {
    return binary_list.size();
  }
};

int
main ()
{
  BinaryList input;

  // read in our input
  std::bitset<12> num;
  while (std::cin >> num)
    input.insert(num);

  BinaryList o2_rating = input;
  BinaryList scrubber_rating = input;

  for (int i = 11; i >= 0 && (o2_rating.size() > 1 || scrubber_rating.size() > 1); --i)
    {
      std::bitset<12> o2_gamma = o2_rating.calc_gamma();
      std::bitset<12> scrubber_epsilon = scrubber_rating.calc_epsilon();
      BinaryList::iterator it;

      // keep only numbers with the most common bit in the current position
      it = o2_rating.begin();
      while (it != o2_rating.end() && o2_rating.size() > 1)
        {
          if ((*it).test(i) == o2_gamma.test(i))
            ++it;
          else
            it = o2_rating.erase(it);
        }

      // keep only numbers with the least common bit in the curren position
      it = scrubber_rating.begin();
      while (it != scrubber_rating.end() && scrubber_rating.size() > 1)
        {
          if ((*it).test(i) == scrubber_epsilon.test(i))
            ++it;
          else
            it = scrubber_rating.erase(it);
        }
    }

  std::cout << o2_rating.front().to_ulong() * scrubber_rating.front().to_ulong() << '\n';
  
  return 0;
}
