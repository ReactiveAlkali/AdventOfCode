#include <iostream>
#include <string>

int
main()
{
  int horizontal_position = 0;
  int depth = 0;
  int aim = 0;
  
  std::string command;
  int value;

  while(std::cin >> command >> value)
    {
      if (command == "forward")
        {
          horizontal_position += value;
          depth += aim * value;
        }
      else if (command == "up")
        aim -= value;
      else if (command == "down")
        aim += value;
      else
        return 1;
    }

  int result = horizontal_position * depth;
  std::cout << result << '\n';

  return 0;
}
