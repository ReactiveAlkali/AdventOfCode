import std.container : SList;

class Directory
{
  Directory[string] subDirectories;
  Directory parent;
  uint size;

  this (Directory parent)
  {
    this.parent = parent;
  }
}

Directory changeDirectory (string arg, Directory cwd)
{
  switch (arg)
    {
    case "..":
      cwd.parent.size += cwd.size;
      cwd = cwd.parent;
      break;
    case "/":
      while (cwd.parent !is null)
        cwd = changeDirectory("..", cwd);
      break;
    default:
      cwd.subDirectories[arg] = new Directory(cwd);
      cwd = cwd.subDirectories[arg];
    }
  return cwd;
}

uint solvePart1 (Directory cwd)
{
  uint sum = cwd.size <= 100000 ? cwd.size : 0;
  foreach (directory; cwd.subDirectories)
    sum += solvePart1(directory);
  return sum;
}

uint solvePart2 (Directory cwd, uint freeSpace)
{  
  uint result = (freeSpace + cwd.size) >= 30_000_000 ? cwd.size : uint.max;
  foreach (dir; cwd.subDirectories)
    {
      import std.algorithm : min;
      result = min(result, solvePart2(dir, freeSpace));
    }
  return result;
}

void main ()
{
  import std.stdio, std.string;

  Directory cwd = new Directory(null);

  // construct our directory
  string line;
  while ((line = readln.strip) !is null)
    {
      switch (line[0..4])
        {
        case "$ ls":
        case "dir ":
          continue;
        case "$ cd":
          cwd = changeDirectory(line[5..$], cwd);
          break;
        default:
          string temp;
          uint fileSize;

          import std.format;
          line.formattedRead("%s %s", fileSize, temp);

          cwd.size += fileSize;
        }
    }
  cwd = changeDirectory("/", cwd);

  solvePart1(cwd).writeln;
  solvePart2(cwd, 70_000_000 - cwd.size).writeln;
}
