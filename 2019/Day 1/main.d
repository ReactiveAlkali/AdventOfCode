import std.stdio;

int[]
parse_input ()
{
  int[] masses;
  int mass;

  while (readf(" %s", mass))
    masses ~= mass;

  return masses;
}

int
mass_fuel_requirement (int mass)
{
  int fuel = mass / 3 - 2;

  if (fuel <= 0)
    fuel = 0;
  else
    fuel += mass_fuel_requirement(fuel);
  
  return fuel;
}

void
main ()
{
  int[] masses = parse_input();

  int sum1;
  foreach (mass; masses)
    sum1 += mass / 3 - 2;
  writeln("Part 1: ", sum1);

  int sum2;
  foreach (mass; masses)
    sum2 += mass_fuel_requirement(mass);
  writeln("Part 2: ", sum2);
}
