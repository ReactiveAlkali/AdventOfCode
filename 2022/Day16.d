import std.algorithm;
import std.array;
import std.container.rbtree;
import std.container.slist;
import std.stdio;
import std.typecons;

struct Valve
{
  int flowRate;
  string[] adjacentValves;
}

Valve[string] parseInput()
{
  import std.format, std.string;

  Valve[string] valves;

  // Read all the input lines and extract relevant information
  string line;
  while ((line = readln.strip) !is null)
    {
      string label;
      int flowRate;
      string adjacent;
      try
	{
	  line.formattedRead("Valve %s has flow rate=%s; tunnels lead to valves %s", label,
			     flowRate, adjacent);
	}
      catch(FormatException e)
	{
	  line.formattedRead(" leads to valve %s", adjacent);
	}
      valves[label] = Valve(flowRate, adjacent.split(", "));
    }
 
  return valves;
}

int[string][string] generateDistanceMatrix (Valve[string] valves)
{
  int[string][string] distances;

  // Initialize the distance matrix
  foreach (valve, valveData; valves)
    {
      // Make sure there's an entry for every valve pair
      foreach (valve2, valveData2; valves)
	{
	  if (valve == valve2)
	    distances[valve][valve2] = 0;
	  else
	    distances[valve][valve2] = int.max / 2; // "infinity"
	}
      // Set the distance for an adjacent valve
      foreach (adjacentValve; valveData.adjacentValves)
	{
	  distances[valve][adjacentValve] = 1;
	}
    }

  // Calculate the distance between each node (Floyd Warshall)
  foreach (valve, valveData; valves)
    {
      foreach (i, row; distances)
	{
	  foreach (j, distance; row)
	    {
	      import std.algorithm : min;
	      distances[i][j] = min(distances[i][j], distances[i][valve] + distances[valve][j]);
	    }
	}
    }
  
  return distances;
}

// Calculate the maximum pressure release of possible sets of open valves
int[RedBlackTree!string] calcRelief(Valve[string] valves, int[string][string] distances,
				    int timeLimit)
{
  int[RedBlackTree!string] maxReliefs;
  
  // Initialize our stack for DFS
  alias State = Tuple!(string, "currValve", int, "remainingTime", int, "totalReleased",
		       RedBlackTree!string, "openValves");
  auto stack = new SList!State(State("AA", timeLimit, 0, new RedBlackTree!string()));

  while (!stack.empty)
    {
      bool hasNextState;
      State currState = stack.front;
      stack.removeFront();

      if (currState.remainingTime == 0)
	continue;

      // Add unopened valves that can be opened in the remaining time to the stack
      foreach (valve, distance; distances[currState.currValve])
	{
	  if (distance < currState.remainingTime && valves[valve].flowRate > 0
	      && valve !in currState.openValves)
	    {
	      // Add potential future state to the queue
	      int remainingTime = currState.remainingTime - distance - 1;
	      int released = currState.totalReleased + remainingTime * valves[valve].flowRate;
	      auto open = currState.openValves.dup;
	      open.insert(valve);
	      stack.insert(State(valve, remainingTime, released, open.dup));

	      // Update the max relief value for the set of open valves if necessary
	      if (open !in maxReliefs || maxReliefs[open] < released)
		maxReliefs[open] = released;

	      hasNextState = true;
	    }
	}
    }

  return maxReliefs;
}

void main()
{
  Valve[string]  valves = parseInput();
  int[string][string] distances = generateDistanceMatrix(valves);

  // Part 1
  int[RedBlackTree!string] maxReliefSets = calcRelief(valves, distances, 30);
  writeln("Part 1: ", maxReliefSets.values.maxElement);

  // Part 2
  maxReliefSets = calcRelief(valves, distances, 26);
  int maxRelief;
  foreach (set0, relief0; maxReliefSets)
    {
      foreach (set1, relief1; maxReliefSets)
	{
	  if (setIntersection(set0[], set1[]).empty && relief0 + relief1 > maxRelief)
	    maxRelief = relief0 + relief1;
	}
    }
  writeln("Part 2: ", maxRelief);
}
