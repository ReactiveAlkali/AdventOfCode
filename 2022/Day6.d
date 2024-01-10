import std.stdio;
import std.string;

enum numDistinct = 14;

void main ()
{
  string input = readln.strip;

  // initialize our window with the first few characters of our string
  int[char] window;
  int index;
  for (; index < numDistinct - 1; ++index)
    window[input[index]]++;

  bool foundStartOfPacket = false;
  for (; index < input.length && !foundStartOfPacket; ++index)
    {
      window[input[index]]++;
      foreach (count; window)
        {
          if (count > 1)
            {
              foundStartOfPacket = false;
              break;
            }
          foundStartOfPacket = true;
        }
      window[input[index-numDistinct+1]]--;
    }
  writeln(index);
}
