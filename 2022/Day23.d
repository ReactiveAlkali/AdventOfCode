import std.array;
import std.container.dlist;
import std.container.rbtree;
import std.conv;
import std.stdio;
import std.string;

enum Direction { north, south, west, east }

bool hasNeighbour(int[2] elf, RedBlackTree!(int[2]) elves)
{
  for (int i = elf[0] - 1; i <= elf[0] + 1; ++i)
    {
      for (int j = elf[1] - 1; j <= elf[1] + 1; ++j)
	{
	  if ([i, j].staticArray in elves && (i != elf[0] || j != elf[1]))
	    return true;
	}
    }
  return false;
}

int[2] proposeMove(int[2] elf, RedBlackTree!(int[2]) elves, DList!Direction directions)
{
  foreach (direction; directions)
    {
      bool willMove = true;
      
      final switch (direction)
	{
	case Direction.north, Direction.south:
	  int row = elf[0] + (direction == Direction.north ? -1 : 1);
	  for (int j = elf[1] - 1; j <= elf[1] + 1 && willMove; ++j)
	    willMove = [row, j].staticArray !in elves;
	  if (willMove)
	    return [row, elf[1]].staticArray;
	  break;
	  
	case Direction.east, Direction.west:
	  int col = elf[1] + (direction == Direction.west ? -1 : 1);
	  for (int i = elf[0] - 1; i <= elf[0] + 1 && willMove; ++i)
	    willMove = [i, col].staticArray !in elves;
	  if (willMove)
	    return [elf[0], col].staticArray;
	  break;
	}
    }
  return elf;
}

size_t emptyTiles(RedBlackTree!(int[2]) elves)
{
  int maxRow = int.min, minRow = int.max;
  int maxCol = int.min, minCol = int.max;

  foreach (elf; elves)
    {
      maxRow = elf[0] > maxRow ? elf[0] : maxRow;
      maxCol = elf[1] > maxCol ? elf[1] : maxCol;
      minRow = elf[0] < minRow ? elf[0] : minRow;
      minCol = elf[1] < minCol ? elf[1] : minCol;
    }

  int numRows = maxRow - minRow + 1;
  int numCols = maxCol - minCol + 1;
  int numCells = numRows * numCols;

  return numCells - elves.length;
}

void main()
{
  auto directions = DList!Direction(Direction.north, Direction.south,
				    Direction.west, Direction.east);
  RedBlackTree!(int[2]) elves = new RedBlackTree!(int[2])();

  int row;
  string line;
  while ((line = readln().strip) !is null)
    {
      foreach (col, cell; line)
	{
	  if (cell == '#')
	    elves.insert([row, to!int(col)].staticArray);
	}
      ++row;
    }

  size_t numRounds;
  while (true)
    {
      ++numRounds;
      int[2][int[2]] proposedMoves;
  
      // Determine where each elf is proposing to move
      foreach (elf; elves)
	{
	  if (hasNeighbour(elf, elves))
	    proposedMoves[elf] = proposeMove(elf, elves, directions);
	}
      if (proposedMoves.empty)
	break;

      // Move the elves
      foreach (currLocation, proposedLocation; proposedMoves)
	{
	  if (elves.insert(proposedLocation))
	    elves.removeKey(currLocation);
	  else
	    {
	      // Handle conflicting move proposals
	      int[2] moveConflicting = proposedLocation[] * 2 - currLocation[];
	      elves.removeKey(proposedLocation);
	      elves.insert(moveConflicting);
	    }
	}

      // Cycle what direction is considered first
      auto temp = directions.front;
      directions.removeFront;
      directions.insert(temp);

      if (numRounds == 10)
	writeln("Part 1: ", emptyTiles(elves));
    }
  writeln("Part 2: ", numRounds);
}
