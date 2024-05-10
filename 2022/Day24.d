import std.container.array;
import std.container.binaryheap;
import std.stdio;
import std.string;

enum Move { right, down, left, up, wait }

struct State
{
  import std.math : abs;
  
  long row;
  long col;
  long time;
  long distance;

  this(long row, long col, long time, long[2] goal)
  {
    this.row = row;
    this.col = col;
    this.time = time;
    this.distance = abs(row - goal[0]) + abs(col - goal[1]);
  }

  this(State state, Move move, long[2] goal)
  {
    final switch (move)
      {
      case Move.right:
	this.row = state.row;
	this.col = state.col + 1;
	break;
      case Move.down:
	this.row = state.row + 1;
	this.col = state.col;
	break;
      case Move.up:
	this.row = state.row - 1;
	this.col = state.col;
	break;
      case Move.left:
	this.row = state.row;
	this.col = state.col - 1;
	break;
      case Move.wait:
	this.row = state.row;
	this.col = state.col;
	break;
      }
    this.time = state.time + 1;
    distance = abs(row - goal[0]) + abs(col - goal[1]);
  }

  // Check whether the state is valid on the given board
  bool isValid(string[] board)
  { 
    long vCols = long(board[0].length) - 2;
    long vRows = long(board.length) - 2;
    
    return col > 0 && col <= vCols && row >= 0 && row < board.length
      && board[row][col] != '#'
      && board[row][(col + time - 1) % vCols + 1] != '<'
      && board[row][(((col - time - 1) % vCols) + vCols) % vCols + 1] != '>'
      && board[(row + time - 1) % vRows + 1][col] != '^'
      && board[(((row - time - 1) % vRows) + vRows) % vRows + 1][col] != 'v';
  }

  long opCmp()(auto ref const State rhs) const
  {
    return (time + distance) - (rhs.time + rhs.distance);
  }
}

long simulateBasin(long[2] start, long[2] goal, long initTime, string[] board)
{
  Array!State states;
  auto stateQueue = heapify!"a > b"(states);
  bool[State] visitedStates;

  // Prepare the initial state
  auto initState = State(start[0], start[1], initTime, goal);
  stateQueue.insert(initState);
  visitedStates[initState] = true;

  // Traverse the basin to the goal
  while (true)
    {
      State nextState;
      State currState = stateQueue.front;
      stateQueue.removeFront;

      if (currState.row == goal[0] && currState.col == goal[1])
	return currState.time;

      // Attempt movements in each direction
      foreach (movement; [Move.down, Move.right, Move.wait, Move.up, Move.left])
	{
	  nextState = State(currState, movement, goal);
	  if (nextState.isValid(board) && nextState !in visitedStates)
	    {
	      stateQueue.insert(nextState);
	      visitedStates[nextState] = true;
	    }
	}
    }
}

void main()
{
  string[] board;

  // Read our puzzle input from stdin
  string line;
  while ((line = readln().strip) !is null)
    board ~= line;

  // The start and end locations
  long[2] start = [0, board[0].indexOf('.')];
  long[2] end = [long(board.length) - 1, board[$-1].indexOf('.')];

  // Part 1
  long minutes = simulateBasin(start, end, 0, board);
  writeln("Part 1: ", minutes);

  // Part 2
  minutes = simulateBasin(end, start, minutes, board);
  writeln("Part 2: ", simulateBasin(start, end, minutes, board));
  
}
