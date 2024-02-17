import std.array;
import std.container.slist;
import std.container.rbtree;
import std.format;
import std.stdio;
import std.string;

// Calculates the surface area the lava droplet
int part1(RedBlackTree!(int[3]) lava)
{
  int surfaceArea;
  foreach (lavaCube; lava)
    {
      for (int i; i < 3; ++i)
	{
	  auto side1 = lavaCube;
	  auto side2 = lavaCube;
	  side1[i] += 1;
	  side2[i] -= 1;

	  if (side1 !in lava)
	    ++surfaceArea;
	  if (side2 !in lava)
	    ++surfaceArea;
	}
    }
  return surfaceArea;
}

// Calculates the external surface area of the lava droplet using a flood fill
int part2(RedBlackTree!(int[3]) lava, int[3] minimums, int[3] maximums)
{
  int surfaceArea;
  RedBlackTree!(int[3]) visited = new RedBlackTree!(int[3]);
  SList!(int[3]) stack;
  stack.insert(minimums);
  while (!stack.empty)
    {
      int[3] coord = stack.front;
      stack.removeFront;
      if (coord !in lava && coord !in visited && withinBounds(coord, minimums, maximums))
	{
	  visited.insert(coord);
	  for (int i; i < 3; ++i)
	    {
	      int[3] side1 = coord, side2 = coord;
	      --side1[i];
	      ++side2[i];
	      
	      if (side1 in lava)
		++surfaceArea;
	      else if (side1 !in visited)
		stack.insert(side1);
	      if (side2 in lava)
		++surfaceArea;
	      else if (side2 !in visited)
		stack.insert(side2);
	    }
	}
    }
  return surfaceArea;
}

// Checks whether the given coordinate is in the given bounds
bool withinBounds(int[3] coord, int[3] minimums, int[3] maximums)
{
  bool result = true;
  for (int i; i < 3; ++i)
    result = result && coord[i] <= maximums[i] && coord[i] >= minimums[i];
  return result;
}

void main()
{
  RedBlackTree!(int[3]) lava = new RedBlackTree!(int[3]);

  // The max and minimum coordinate along each axis
  int[3] minimums = int.max;
  int[3] maximums = int.min;

  // Parse in the input file
  string line;
  while ((line = readln.strip) !is null)
    {
      // Place the cube onto the grid
      int[3] coords;
      line.formattedRead("%s,%s,%s", coords[0], coords[1], coords[2]);
      lava.insert(coords);

      // Record the max and min along each axis
      for (int i; i < 3; ++i)
	{
	  if (coords[i] < minimums[i])
	    minimums[i] = coords[i];
	  if (coords[i] > maximums[i])
	    maximums[i] = coords[i];
	}
    }

  // We want the maximum and minimums to be one outside the actual lava locations
  for (int i; i < 3; ++i)
    {
      --minimums[i];
      ++maximums[i];
    }

  writeln("Part 1: ", part1(lava));
  writeln("Part 2: ", part2(lava, minimums, maximums));
}
