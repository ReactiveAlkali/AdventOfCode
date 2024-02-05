import std.container.rbtree;
import std.stdio;
import std.string;

class Rock
{
  ulong row, col;
  ulong height;

  bool move(char direction, bool[7][] chamber)
  {
    if (!collision(direction, chamber))
      {
	final switch (direction)
	  {
	  case '>': col++; break;
	  case '<': col--; break;
	  case 'v': row--; break;
	  }
	return true;
      }
    return false;
  }

  abstract bool collision(char direction, bool[7][] chamber);

  abstract bool[7][] addToChamber(bool[7][] chamber);
}

// ####
class LineRock : Rock
{
  this()
  {
    height = 1;
  }
  
  override bool collision(char direction, bool[7][] chamber)
  {
    return (direction == '>' && (col == 3 || chamber[row][col+4]))
      || (direction == '<' && (col == 0 || chamber[row][col-1]))
      || (direction == 'v' && (row == 0 || collisionBelow(chamber)));
  }

  private bool collisionBelow(bool[7][] chamber)
  {
    bool collision;
    for (ulong i = col; !collision && i < col + 4; ++i)
      collision = chamber[row-1][i];
    return collision;
  }

  override bool[7][] addToChamber(bool[7][] chamber)
  {
    for (ulong i = col; i < col + 4; ++i)
      chamber[row][i] = true;
    return chamber;
  }
}

//  #
// ###
//  #
class PlusRock : Rock
{
  this()
  {
    height = 3;
  }

  override bool collision(char direction, bool[7][] chamber)
  {
    return (direction == '>' && (col == 4 || chamber[row][col+2] || chamber[row+1][col+3]
				 || chamber[row+2][col+2]))
      || (direction == '<' && (col == 0 || chamber[row][col] || chamber[row+1][col-1]
			       || chamber[row+2][col]))
      || (direction == 'v' && (row == 0 || chamber[row][col] || chamber[row-1][col+1]
			       || chamber[row][col+2]));
  }

  override bool[7][] addToChamber(bool[7][] chamber)
  {
    chamber[row][col+1] = true;
    for (ulong i = col; i < col + 3; ++i)
      chamber[row+1][i] = true;
    chamber[row+2][col+1] = true;
    return chamber;
  }
}

//   #
//   #
// ###
class RightRock : Rock
{
  this()
  {
    height = 3;
  }

  override bool collision(char direction, bool[7][] chamber)
  {
    return (direction == '>' && (col == 4 || collisionRight(chamber)))
      || (direction == '<' && (col == 0 || chamber[row][col-1] || chamber[row+1][col+1]
			       || chamber[row+2][col+1]))
      || (direction == 'v' && (row == 0 || collisionBelow(chamber)));
  }

  private bool collisionBelow(bool[7][] chamber)
  {
    bool collision;
    for (ulong i = col; !collision && i < col + 3; ++i)
      collision = chamber[row-1][i];
    return collision;
  }

  private bool collisionRight(bool[7][] chamber)
  {
    bool collision;
    for (ulong i = row; !collision && i < row + 3; ++i)
      collision = chamber[i][col+3];
    return collision;
  }

  override bool[7][] addToChamber(bool[7][] chamber)
  {
    for (ulong i = col; i < col + 3; ++i)
      chamber[row][i] = true;
    chamber[row+1][col+2] = true;
    chamber[row+2][col+2] = true;
    return chamber;
  }
}

// #
// #
// #
// #
class VerticalRock : Rock
{
  this()
  {
    height = 4;
  }

  override bool collision(char direction, bool[7][] chamber)
  {
    return (direction == '>' && (col == 6 || sideCollision(chamber, '>')))
      || (direction == '<' && (col == 0 || sideCollision(chamber, '<')))
      || (direction == 'v' && (row == 0 || chamber[row-1][col]));
  }

  private bool sideCollision(bool[7][] chamber, char direction)
  {
    bool collision;
    ulong col = direction ==  '>' ? this.col + 1 : this.col - 1;
    for (ulong i = row; !collision && i < row + height; ++i)
      collision = chamber[i][col];
    return collision;
  }

  override bool[7][] addToChamber(bool[7][] chamber)
  {
    for (ulong i = row; i < row + height; ++i)
      chamber[i][col] = true;
    return chamber;
  }
}

// ##
// ##
class SquareRock : Rock
{
  this()
  {
    height = 2;
  }

  override bool collision(char direction, bool[7][] chamber)
  {
    return (direction == '>' && (col == 5 || chamber[row][col+2] || chamber[row+1][col+2]))
      || (direction == '<' && (col == 0 || chamber[row][col-1] || chamber[row+1][col-1]))
      || (direction == 'v' && (row == 0 || chamber[row-1][col] || chamber[row-1][col+1]));
  }

  override bool[7][] addToChamber(bool[7][] chamber)
  {
    chamber[row][col] = true;
    chamber[row][col+1] = true;
    chamber[row+1][col] = true;
    chamber[row+1][col+1] = true;
    return chamber;
  }
}

class RockSimulation
{
  // Simulation constants
  string jetPattern;
  bool[7][] chamber = new bool[7][](4);
  Rock[5] rocks = [new LineRock(), new PlusRock(), new RightRock(), new VerticalRock(),
		   new SquareRock()];
  ulong[4][string] states;

  ulong towerHeight;
  int jetIndex;
  int rockIndex;

  this(string jetPattern)
  {
    this.jetPattern = jetPattern;
  }

  void simulateFallingRock(Rock rock)
  {
    while (true)
      {
	rock.move(jetPattern[jetIndex++], chamber);
	if (jetIndex >= jetPattern.length)
	  jetIndex = 0;
	if (!rock.move('v', chamber))
	  break;
      }
    rock.addToChamber(chamber);
    if (rock.row + rock.height > towerHeight)
      towerHeight = rock.row + rock.height;
  }

  Rock nextRock()
  {
    Rock rock = rocks[rockIndex++];
    if (rockIndex >= 5)
      rockIndex = 0;

    if (towerHeight + rock.height + 3 > chamber.length)
      chamber.length *= 2;
    rock.row = towerHeight + 3;
    rock.col = 2;
    return rock;
  }

  ulong run(ulong steps, bool cycleDetection = false)
  {
    ulong heightOfCycles;
   
    for (ulong i; i < steps; ++i)
      {
	auto rock = rockIndex;
	ulong prevHeight = towerHeight;
	simulateFallingRock(nextRock());

	// Update the state with the top of the tower
	string towerTop = "";
	for (size_t row = towerHeight; row > towerHeight - 8; --row)
	  {
	    foreach (cell; chamber[row])
	      towerTop ~= cell ? "1" : "0";
	  }
	string state = format("%s-%s-%s", rock, jetIndex, towerTop);

	// Handle the detection of cycles
	if (state in states && cycleDetection && i > 8)
	  {
	    auto heightPerCycle = towerHeight - states[state][0];
	    auto rocksPerCycle = i - states[state][1];

	    // Adjust our step "adding" our cycles to the chamber
	    auto repeatsLeft = (steps - i) / rocksPerCycle;
	    i += rocksPerCycle * repeatsLeft;

	    // Add the height of all the cycles to the height of the tower
	    heightOfCycles += repeatsLeft * heightPerCycle;

	    states.clear;
	  }
	else
	  states[state] = [towerHeight, i, prevHeight, i-1];
      }
    
    return towerHeight + heightOfCycles;
  }
}

void main()
{
  string jetPattern = readln().strip;
  auto rockSim = new RockSimulation(jetPattern);
  writeln("Part 1: ", rockSim.run(2022));

  rockSim = new RockSimulation(jetPattern);
  writeln("Part 2: ", rockSim.run(1000000000000, true));
}
