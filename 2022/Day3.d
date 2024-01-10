import std.stdio;
import std.string;

int toPriority (char item)
{
  import std.ascii : isUpper;

  int priority;
  if (item.isUpper)
    priority = item - 38;
  else
    priority = item - 96;
  return priority;
}

void main ()
{
  int prioritySum;
  
  ubyte[][3] elves;
  while ((elves[0] = readln.strip.dup.representation) !is null)
    {
      // read in the rest of the group
      elves[1] = readln.strip.dup.representation;
      elves[2] = readln.strip.dup.representation;

      import std.algorithm : setIntersection, sort;
      import std.conv : to;
      auto intersection = setIntersection(setIntersection(elves[0].sort, elves[1].sort), elves[2].sort);
      prioritySum += intersection.front.to!char.toPriority;
    }

  writeln(prioritySum);
}
