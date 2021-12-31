#include <iostream>
#include <cstdio>
#include <string>
#include <array>
#include <list>
#include <algorithm>
#include <iterator>

struct Cuboid
{
  long x1, x2;
  long y1, y2;
  long z1, z2;
  bool on {};

  bool
  collision (const Cuboid& other)
  {
    return other.x1 < x2 && other.x2 > x1 && other.y1 < y2 && other.y2 > y1
      && other.z1 < z2 && other.z2 > z1;
  }

  Cuboid
  intersection (const Cuboid& other)
  {
    return Cuboid { std::max(x1, other.x1), std::min(x2, other.x2),
      std::max(y1, other.y1), std::min(y2, other.y2), std::max(z1, other.z1),
      std::min(z2, other.z2), !other.on };
  }

  long
  calc_area ()
  {
    long area = (x2 - x1 + 1) * (y2 - y1 + 1) * (z2 - z1 + 1);
    if (!on)
      area = -area;
    return area;
  }
};

std::list<Cuboid>
parse_input ()
{
  std::list<Cuboid> steps {};
  std::string input {};

  while (std::cin >> input)
    {
      Cuboid step {};
      step.on = input == "on";

      std::getline(std::cin, input);
      std::sscanf(input.c_str(), " x=%ld..%ld,y=%ld..%ld,z=%ld..%ld", &step.x1,
                  &step.x2, &step.y1, &step.y2, &step.z1, &step.z2);

      steps.push_back(step);
    }

  return steps;
}

int
main ()
{
  std::list<Cuboid> steps { parse_input() };
  std::list<Cuboid> final_cuboids {};

  while (!steps.empty())
    {
      std::list<Cuboid>::iterator it { final_cuboids.begin() };

      while (it != final_cuboids.end())
        {
          if (steps.front().collision(*it))
            final_cuboids.push_front(steps.front().intersection(*it));
          ++it;
        }
      
      if (steps.front().on)
        final_cuboids.push_front(steps.front());
      steps.pop_front();
    }

  long area {};
  for (Cuboid c : final_cuboids)
    area += c.calc_area();
  std::cout << area << '\n';

  return 0;
}
