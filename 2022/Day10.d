import std.stdio, std.string, std.format;

void main ()
{
  char[240] crt = '.';

  int sum;
  int recordCycle = 20;
  int clock;
  int x = 1;
  
  string line;
  while ((line = readln.strip) !is null)
    {
      // parse the recieved instruction
      int temp;
      int incr;
      if (line == "noop")
        incr = 1;
      else
        {
          incr = 2;
          line.formattedRead("addx %s", temp);
        }

      // increment the clock cycles
      int startClock = clock;
      for (;clock < startClock + incr; ++clock)
        {
          if ((clock % 40) >= x-1 && (clock % 40) <= x+1)
            crt[clock] = '#';
        }      

      // part a
      if (clock >= recordCycle)
        {
          sum += recordCycle * x;
          recordCycle += 40;
        }
      x += temp;
    }

  sum.writeln;

  foreach (i, pixel; crt)
    {
      pixel.write;
      if (!((i+1) % 40))
        '\n'.write;
    }
}
