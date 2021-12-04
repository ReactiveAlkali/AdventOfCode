#include <iostream>
#include <bitset>
#include <array>

int
main()
{
  int total_nums = 0;
  std::array<int, 12> set_count{};
  std::bitset<12> num;

  // count the number of bits set at a given location
  while (std::cin >> num)
    {
      for (int i = 0; i < 12; ++i)
        {
          if (num.test(i))
            set_count[i]++;
        }
      ++total_nums;
    }
  
  // find the gamma and epsilon values
  std::bitset<12> gamma;
  for (int i = 0; i < 12; ++i)
    {
      gamma.set(i, set_count[i] > total_nums / 2);
    }
  std::bitset<12> epsilon = gamma;
  epsilon.flip();

  std::cout << gamma.to_ulong() * epsilon.to_ulong() << '\n';
  return 0;
}
