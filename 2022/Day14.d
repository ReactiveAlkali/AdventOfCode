import std.complex;
import std.container.rbtree;
import std.stdio;
import std.typecons;

alias Coord = Tuple!(int, "x", int, "y");

void drawLine (ref bool[Coord] grid, Coord start, Coord end)
{
  import std.algorithm : swap;
  if (start.x == end.x) //vertical
    {
      if (start.y > end.y)
        swap(start.y, end.y);
      foreach (y; start.y..end.y+1)
        grid[Coord(start.x, y)] = true;
    }
  else //horizontal
    {
      if (start.x > end.x)
        swap(start.x, end.x);
      foreach (x; start.x..end.x+1)
        grid[Coord(x, start.y)] = true;
    }
}

Tuple!(bool[Coord], int) parseInput ()
{
  import std.array, std.format, std.string;

  bool[Coord] grid;
  int maxY = int.min;
  
  string line;
  while ((line = readln.strip) !is null)
    {
      auto coords = line.split(" -> ");

      int prevX = int.min;
      int prevY = int.min;
      foreach (coord; coords)
        {
          int x, y;
          coord.formattedRead("%s,%s", x, y);

          if (prevX != int.min && prevY != int.min)
            drawLine(grid, Coord(prevX, prevY), Coord(x, y));

          prevX = x;
          prevY = y;

          if (y > maxY)
            maxY = y;
        }
    }

  return tuple(grid, maxY);
}

void printGrid (bool[Coord] grid)
{
  for (int y = 0; y < 10; ++y)
    {
      for (int x = 494; x < 504; ++x)
        {
          bool* p = Coord(x, y) in grid;
          if (p !is null)
            {
              if (*p)
                write('#');
              else
                write('o');
            }
          else
            write('.');
        }
      writeln;
    }
}

bool simulateSand (ref bool[Coord] grid, int floor)
{
  Coord currPos = Coord(500, 0);
  bool onFloor;
  while (!onFloor)
    {
      if (!(Coord(currPos.x, currPos.y + 1) in grid))
        currPos = Coord(currPos.x, currPos.y + 1);
      else if (!(Coord(currPos.x - 1, currPos.y + 1) in grid))
        currPos = Coord(currPos.x - 1, currPos.y + 1);
      else if (!(Coord(currPos.x + 1, currPos.y + 1) in grid))
        currPos = Coord(currPos.x + 1, currPos.y + 1);
      else
        {
          grid[currPos] = false;
          break;
        }

      // check whether we're on the floor
      if (currPos.y + 1 == floor)
        {
          onFloor = true;
          grid[currPos] = false;
        }
    }
  return currPos == Coord(500, 0);
}

void main ()
{
  auto input = parseInput();
  auto grid = input[0];
  int maxY = input[1];
  
  int count;
  bool blocked;
  for (; !blocked; ++count)
    blocked = simulateSand(grid, maxY + 2);
  writeln(count);

  //printGrid(grid);
}
