#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <set>
#include <array>
#include <bitset>

constexpr int STEPS = 50;

struct Image
{
  int i1, i2;
  int j1, j2;
  bool border { false };
  std::set<std::array<int,2>> light_pixels{};
};

std::tuple<std::vector<bool>,Image>
parse_input ()
{
  std::string input_line {};

  std::cin >> input_line;
  std::vector<bool> enhancement_algorithm {};
  for (char c : input_line)
    enhancement_algorithm.push_back(c == '#');

  Image input_image {};

  int i {};
  int j {};
  while (std::cin >> input_line)
    {
      std::vector<bool> row {};

      j = 0;
      for (char c : input_line)
        {
          if (c == '#')
            input_image.light_pixels.insert(std::array<int,2>{i,j});
          ++j;
        }
      ++i;
    }
  input_image.i1 = 0;
  input_image.i2 = i - 1;
  input_image.j1 = 0;
  input_image.j2 = j - 1;

  return std::make_tuple(enhancement_algorithm, input_image);
}

unsigned long
get_enhancement_index (const Image& image, int row, int col)
{
  std::bitset<9> index {};
  
  for (int i { row - 1 }; i <= row + 1; ++i)
    {
      for (int j { col - 1 }; j <= col + 1; ++j)
        {
          // We check whether the current pixel is within the bounds of the
          // image.
          bool light { (j >= image.j1 && j <= image.j2 && i >= image.i1
                        && i <= image.i2)
            ? image.light_pixels.count(std::array<int,2>{i,j}) == 1
            : image.border };
          index <<= 1;
          index.set(0, light);
        }
    }

  return index.to_ulong();
}

Image
enhance (const std::vector<bool>& algorithm,
         const Image& image)
{
  Image enhanced_image { image.i1 - 1, image.i2 + 1, image.j1 - 1, image.j2 + 1 };

  for (int i { enhanced_image.i1 }; i <= enhanced_image.i2; ++i)
    {
      for (int j { enhanced_image.j1 }; j <= enhanced_image.j2; ++j)
        {
          unsigned long index { get_enhancement_index(image, i, j) };
          if (algorithm[index])
            enhanced_image.light_pixels.insert(std::array<int,2>{i,j});
        }
    }

  // All pixels in the border region only have two possible values
  if (image.border)
    enhanced_image.border = algorithm.back();
  else
    enhanced_image.border = algorithm.front();

  return enhanced_image;
}

int
main ()
{
  std::vector<bool> enhancement_algorithm;
  Image input_image;
  std::tie(enhancement_algorithm, input_image) = parse_input();

  for (int i {}; i < STEPS; ++i)
    input_image = enhance(enhancement_algorithm, input_image);

  std::cout << input_image.light_pixels.size() << '\n';

  return 0;
}
