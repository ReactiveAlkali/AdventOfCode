import std.conv : to;
import std.stdio;

struct Monkey
{
  ulong[] items;
  char op;
  string operand;
  int test;
  uint trueMonkey;
  uint falseMonkey;
  uint numInspections;

  ulong inspectItem ()
  {
    final switch (op)
      {
      case '+':
        items[0] += operand == "old" ? items[0] : operand.to!long;
        break;
      case '*':
        items[0] *= operand == "old" ? items[0] : operand.to!long;
      }
    ++numInspections;
    return items[0];
  }

  ulong boredom ()
  {
    return items[0] /= 3;
  }

  uint testItem ()
  {
    if (items[0] % test == 0)
      return trueMonkey;
    else
      return falseMonkey;
  }

  void removeItem ()
  {
    items = items[1..$];
  }

  void addItem (ulong item)
  {
    items ~= item;
  }
}

Monkey[] parseInput ()
{
  import std.algorithm;
  import std.array;
  import std.string;

  Monkey[] monkeys;
  string line;
  while ((line = readln.strip) !is null)
    {
      if (line == "")
        continue;
      else if (line.startsWith("Monkey"))
        monkeys.length += 1;
      else
        {
          auto tokens = line.split(":");
          final switch (tokens[0])
            {
            case "Starting items":
              monkeys[$-1].items = tokens[1].split(",").map!(a => a.strip.to!ulong).array;
              break;
            case "Operation":
              monkeys[$-1].op = tokens[1][11];
              monkeys[$-1].operand = tokens[1][13..$];
              break;
            case "Test":
              monkeys[$-1].test = tokens[1][14..$].to!int;
              break;
            case "If true":
              monkeys[$-1].trueMonkey = tokens[1][17..$].to!uint;
              break;
            case "If false":
              monkeys[$-1].falseMonkey = tokens[1][17..$].to!uint;
            }
        }
    }
  return monkeys;
}

ulong calcCommonMultiple (Monkey[] monkeys)
{
  ulong cm = 1;
  foreach (const ref monkey; monkeys)
    cm *= monkey.test;
  return cm;
}

void main ()
{
  auto monkeys = parseInput();
  auto commonMultiple = calcCommonMultiple(monkeys);

  foreach (_; 0..10000)
    {
      foreach (ref monkey; monkeys)
        {
          while (monkey.items.length)
            {
              auto item = monkey.inspectItem;
              //item = monkey.boredom;
              auto m2 = monkey.testItem;
              monkeys[m2].addItem(item % commonMultiple);
              monkey.removeItem;
            }
        }
    }

  import std.algorithm : sort, fold;
  import std.range : take;
    
  uint inspections[];
  foreach (const ref monkey; monkeys)
    inspections ~= monkey.numInspections;
  inspections.sort!("a > b").take(2).fold!((ulong a, ulong b) => a * b).writeln;
}
