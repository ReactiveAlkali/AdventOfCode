import std.container.dlist;
import std.stdio;
import std.string;
import std.typecons;

alias Position = Tuple!(ulong, ulong);

void addNeigbours (const char[][] heightmap, int[][] steps, ref DList!Position q, Position pos)
{
  auto climbHeight = heightmap[pos[0]][pos[1]] + 1;
  auto stepIncr = steps[pos[0]][pos[1]] + 1;
  
  if (pos[0] > 0 && steps[pos[0] - 1][pos[1]] == int.max && heightmap[pos[0] - 1][pos[1]] <= climbHeight)
    {
      q.insert(Position(pos[0] - 1, pos[1]));
      steps[pos[0] - 1][pos[1]] = stepIncr;
    }
  if (pos[0] < heightmap.length - 1 && steps[pos[0] + 1][pos[1]] == int.max
      && heightmap[pos[0] + 1][pos[1]] <= climbHeight)
    {
      q.insert(Position(pos[0] + 1, pos[1]));
      steps[pos[0] + 1][pos[1]] = stepIncr;
    }
  if (pos[1] > 0 && steps[pos[0]][pos[1] - 1] == int.max && heightmap[pos[0]][pos[1] - 1] <= climbHeight)
    {
      q.insert(Position(pos[0], pos[1] - 1));
      steps[pos[0]][pos[1] - 1] = stepIncr;
    }
  if (pos[1] < heightmap[0].length - 1 && steps[pos[0]][pos[1] + 1] == int.max
      && heightmap[pos[0]][pos[1] + 1] <= climbHeight)
    {
      q.insert(Position(pos[0], pos[1] + 1));
      steps[pos[0]][pos[1] + 1] = stepIncr;
    }
}

int findPath (const char[][] heightmap, Position startPos, Position endPos)
{
  int[][] steps = new int[][](heightmap.length, heightmap[0].length);
  foreach (ref row; steps)
    {
      foreach (ref cell; row)
        cell = int.max;
    }
  
  steps[startPos[0]][startPos[1]] = 0;

  auto q = DList!Position(startPos);
  while (steps[endPos[0]][endPos[1]] == int.max && !q.empty)
    {
      auto currPos = q.front;
      q.removeFront;
      addNeigbours(heightmap, steps, q, currPos);
    }
  return steps[endPos[0]][endPos[1]];
}

void main ()
{
  char[][] heightmap;
  Position startPos, endPos;
  Position[] startPositions;

  string line;
  while ((line = readln.strip) !is null)
    {
      char[] row;
      foreach (l; line)
        {
          char letter = l;
          if (letter == 'S')
            {
              letter = 'a';
              startPos[0] = heightmap.length;
              startPos[1] = row.length;
            }
          if (letter == 'E')
            {
              letter = 'z';
              endPos[0] = heightmap.length;
              endPos[1] = row.length;
            }
          if (letter == 'a')
            startPositions ~= Position(heightmap.length, row.length);
          row ~= letter;
        }
      heightmap ~= row;
    }

  //foreach (row; heightmap)
  //  row.length.writeln;

  // part 1
  findPath(heightmap, startPos, endPos).writeln;

  // part 2
  import std.algorithm : map, minElement;
  findPath(heightmap, startPositions[2], endPos);
  startPositions.map!(a => findPath(heightmap, a, endPos)).minElement.writeln;
}
