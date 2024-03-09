import std.conv;
import std.format;
import std.math : abs, round, isNaN;
import std.stdio;
import std.string;
import std.typecons;

alias Monkey = Tuple!(double, "value", string, "left", string, "right", char, "op");

double solveMonkey(string monkeyStr, Monkey[string] monkeys)
{
  Monkey monkey = monkeys[monkeyStr];
  if (monkey.left is null)
    return monkey.value;

  double leftValue = solveMonkey(monkey.left, monkeys);
  double rightValue = solveMonkey(monkey.right, monkeys);
  double value;
  final switch (monkey.op)
    {
    case '+': value = leftValue + rightValue; break;
    case '-': value = leftValue - rightValue; break;
    case '/': value = leftValue / rightValue; break;
    case '*': value = leftValue * rightValue; break;
    }
  monkeys[monkeyStr].value = value;
  return value;
}

double solveLhs (double curr, double right, char op)
{
  double value;
  final switch (op)
    {
    case '+': value = curr - right; break;
    case '-': value = curr + right; break;
    case '*': value = curr / right; break;
    case '/': value = curr * right; break;
    }
  return value;
}

double solveRhs (double curr, double left, char op)
{
  double value;
  final switch (op)
    {
    case '/': value = left / curr; break;
    case '*': value = curr / left; break;
    case '+': value = curr - left; break;
    case '-': value = left - curr; break;
    }
  return value;
}

void main()
{
  Monkey[string] monkeys;
  
  string line;
  while ((line = readln().strip) !is null)
    {
      string monkey, left, right;
      char op;

      uint items = formattedRead(line, "%s: %s %s %s", monkey, left, op, right);
      if (items == 2)
	monkeys[monkey] = Monkey(to!double(left), null, null, ' ');
      else
	monkeys[monkey] = Monkey(double.init, left, right, op);
    }

  writeln("Part 1: ", to!long(solveMonkey("root", monkeys)));

  //
  // Part 2
  //
  monkeys["humn"].value = double.nan;
  double left = solveMonkey(monkeys["root"].left, monkeys);
  double right = solveMonkey(monkeys["root"].right, monkeys);

  string currMonkey;
  if (left.isNaN())
    {
      monkeys[monkeys["root"].left].value = right;
      currMonkey = monkeys["root"].left;
    }
  else
    {
      monkeys[monkeys["root"].right].value = left;
      currMonkey = monkeys["root"].right;
    }

  while (monkeys["humn"].value.isNaN())
    {
      string leftMonkey = monkeys[currMonkey].left;
      string rightMonkey = monkeys[currMonkey].right;
      if (monkeys[leftMonkey].value.isNaN())
	{
	  monkeys[leftMonkey].value = solveLhs(monkeys[currMonkey].value,
					       monkeys[rightMonkey].value,
					       monkeys[currMonkey].op);
	  currMonkey = leftMonkey;
	}
      else
	{
	  monkeys[rightMonkey].value = solveRhs(monkeys[currMonkey].value,
						monkeys[leftMonkey].value,
						monkeys[currMonkey].op);
	  currMonkey = rightMonkey;
	}
    }
  
  writeln("Part 2: ", to!long(monkeys["humn"].value));
}
