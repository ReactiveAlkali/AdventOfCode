import std.container : SList;
import std.stdio;
import std.string;

auto readStacks()()
{
  // read in our starting stacks
  string inputLine;
  string[] lines;
  while ((inputLine = readln.chomp) != "")
    lines ~= inputLine;

  // initialize our stacks
  int numStacks = lines[$-1][$-2] - 48;
  SList!char[] stacks;
  stacks.length = numStacks;

  foreach_reverse (line; lines)
    {
      for (int i = 1; i < line.length; i += 4)
        {
          import std.ascii : isAlpha;
          if (line[i].isAlpha)
            stacks[i/4].insertFront(line[i]);
        }
    }

  return stacks;
}

void main ()
{
  auto stacks = readStacks();

  string inputLine;
  while ((inputLine = readln.strip) !is null)
    {
      // parse the command from input
      import std.format : formattedRead;
      int move, from, to;
      inputLine.formattedRead("move %s from %s to %s", move, from, to);

      // apply the command to the stacks
      SList!char tempStack;
      for (int i = 0; i < move; ++i)
        {
          tempStack.insertFront(stacks[from-1].front);
          stacks[from-1].removeFront;
        }
      while (!tempStack.empty)
        {
          stacks[to-1].insert(tempStack.front);
          tempStack.removeFront;
        }
    }

  foreach (stack; stacks)
    write(stack.front);
  writeln;
}
