import std.stdio;

int[][] parseInput ()
{
  import std.string, std.conv : to;

  int[][] grid;
  string line;
  
  while ((line = readln.strip) !is null)
    {
      int[] row;
      foreach (character; line)
        row ~= character.to!string.to!int;
      grid ~= row;
    }
  return grid;
}

void scanRows (int[][] treeGrid, bool[][] visible)
{
  foreach (rowIndex, row; treeGrid)
    {
      int highestLeft = -1;
      int highestRight = -1;
      
      for (int i = 0; i < row.length && (highestLeft != 9 || highestRight != 9); ++i)
        {
          if (highestLeft < row[i])
            {
              visible[rowIndex][i] = true;
              highestLeft = row[i];
            }
          if (highestRight < row[row.length-i-1])
            {
              visible[rowIndex][row.length-i-1] = true;
              highestRight = row[row.length-i-1];
            }
        }
    }
}

void scanCols (int[][] treeGrid, bool[][] visible)
{
  for(int col; col < treeGrid[0].length; ++col)
    {
      int highestTop = -1;
      int highestBottom = -1;

      for (int i = 0; i < treeGrid.length && (highestTop != 9 || highestBottom != 9); ++i)
        {
          if (highestTop < treeGrid[i][col])
            {
              visible[i][col] = true;
              highestTop = treeGrid[i][col];
            }
          if (highestBottom < treeGrid[treeGrid.length-i-1][col])
            {
              visible[treeGrid.length-i-1][col] = true;
              highestBottom = treeGrid[treeGrid.length-i-1][col];
            }
        }
    }
}

int scanRight (int[][] trees, int row, int col)
{
  int count;
  for (int i = col + 1; i < trees[row].length; ++i)
    {
      ++count;
      if (trees[row][col] <= trees[row][i])
        break;
    }
  return count;
}

int scanLeft (int[][] trees, int row, int col)
{
  int count;
  for (int i = col - 1; i >= 0; --i)
    {
      ++count;
      if (trees[row][col] <= trees[row][i])
        break;
    }
  return count;
}

int scanUp (int[][] trees, int row, int col)
{
  int count;
  for (int i = row - 1; i >= 0; --i)
    {
      ++count;
      if (trees[row][col] <= trees[i][col])
        break;
    }
  return count;
}

int scanDown (int[][] trees, int row, int col)
{
  int count;
  for (int i = row + 1; i < trees.length; ++i)
    {
      ++count;
      if (trees[row][col] <= trees[i][col])
        break;
    }
  return count;
}

int[][] calcScenicScores (int[][] trees)
{
  int[][] scenicScores = new int[][](trees.length, trees[0].length);
  foreach (int i, row; trees)
    {
      foreach (int j, tree; row)
        {
          scenicScores[i][j] = scanUp(trees, i, j) * scanDown(trees, i, j) * scanRight(trees, i, j)
            * scanLeft(trees, i, j);
        }
    }
  return scenicScores;
}

void main ()
{
  int[][] grid = parseInput();
  bool[][] visible = new bool[][](grid.length, grid[0].length);

  scanRows(grid, visible);
  scanCols(grid, visible);

  int sum;
  foreach (row; visible)
    {
      foreach (val; row)
        sum += val;
    }
  sum.writeln;

  auto scenicScores = calcScenicScores(grid);
  int maxScore = int.min;
  foreach (row; scenicScores)
    {
      import std.algorithm.searching : maxElement;
      int rowMax = row.maxElement;
      if (rowMax > maxScore)
        maxScore = rowMax;
    }
  maxScore.writeln;
}
