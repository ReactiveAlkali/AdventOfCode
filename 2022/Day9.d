import std.stdio, std.container.rbtree;

int[2] moveHead (int[2] headPos, char direction)
{
  final switch (direction)
    {
    case 'R':
      ++headPos[0];
      break;
    case 'L':
      --headPos[0];
      break;
    case 'U':
      ++headPos[1];
      break;
    case 'D':
      --headPos[1];
    }
  return headPos;
}

int[2] updateTail (int[2] tailPos, int[2] headPos)
{
  import std.math : sqrt, abs;
  
  int[2] diff = headPos[] - tailPos[];
  auto mag = sqrt(cast(float)diff[0] * diff[0] + diff[1] * diff[1]);
  if (mag == 2)
    diff[] /= 2;
  else if (mag > 2)
    {
      import std.math : abs;
      int[2] temp;
      temp[0] = abs(diff[0]);
      temp[1] = abs(diff[1]);
      diff[] /= temp[];
    }
  else
    diff[] -= diff[];
  
  tailPos[] += diff[];
  return tailPos;
}

void main ()
{
  auto positions = redBlackTree!(int[2])([0,0]);
  int[2][10] knotPositions;

  char direction;
  int steps;
  while (readf(" %s %s", direction, steps))
    {
      foreach (_; 0..steps)
        {
          knotPositions[0] = knotPositions[0].moveHead(direction);
          for (int i = 1; i < 10; ++i)
            knotPositions[i] = knotPositions[i].updateTail(knotPositions[i-1]);
          positions.insert(knotPositions[9]);
        }
    }
  positions.length.writeln;
}
