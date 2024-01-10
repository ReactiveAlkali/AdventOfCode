import std.stdio;
import std.string : strip;
import std.conv : to;
import std.algorithm : sum;
import std.container.binaryheap;
import std.range : take;

void main()
{
  int currCalories;
  int[] calorieList;
  
  string inputLine;
  while ((inputLine = strip(readln())) !is null)
    {
      if (inputLine != "")
        currCalories += to!int(inputLine);
      else
        {
          calorieList ~= currCalories;
          currCalories = 0;
        }
    }
  calorieList ~= currCalories;

  auto topThreeSum = heapify(calorieList).take(3).sum;
  writeln(topThreeSum);
}
