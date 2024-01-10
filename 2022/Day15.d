import std.algorithm;
import std.array;
import std.range;
import std.stdio;
import std.typecons;

enum MinX = 0;
enum MaxX = 4000000;
enum MinY = 0;
enum MaxY = 4000000;

struct Sensor
{
  int x, y;
  int dist;

  this (int x, int y, int beaconX, int beaconY)
  {
    this.x = x;
    this.y = y;

    import std.math : abs;
    dist = distanceTo(beaconX, beaconY);
  }

  int distanceTo (int x, int y)
  {
    import std.math : abs;
    return abs(x - this.x) + abs(y - this.y);
  }
}

Sensor[] parseInput ()
{
  import std.format, std.string;

  Sensor[] sensors;  
  string line;
  while ((line = readln.strip) !is null)
    {
      int x, y, bx, by;
      line.formattedRead("Sensor at x=%s, y=%s: closest beacon is at x=%s, y=%s", x, y, bx, by);
      sensors ~= Sensor(x, y, bx, by);
    }
  return sensors;
}

int scanRow (Sensor[] sensors, int y, int minX, int maxX)
{
  auto ranges = sensors
    .filter!(a => a.distanceTo(a.x, y) < a.dist)
    .map!(a => tuple(a.x - a.dist + a.distanceTo(a.x, y), a.x + a.dist - a.distanceTo(a.x, y)))
    .map!(a => tuple(max(a[0], minX), min(a[1], maxX)))
    .array.sort;

  Tuple!(int, int)[] mergedRanges;
  mergedRanges ~= ranges[0];
  for (int i = 1; i < ranges.length; ++i)
    {
      if (ranges[i][1] < mergedRanges[$-1][0] - 1 || mergedRanges[$-1][1] < ranges[i][0] - 1)
        mergedRanges ~= ranges[i];
      else
        {
          mergedRanges[$-1][0] = min(mergedRanges[$-1][0], ranges[i][0]);
          mergedRanges[$-1][1] = max(mergedRanges[$-1][1], ranges[i][1]);
        }
    }
  //mergedRanges.map!(a => a[1] - a[0]).sum.writeln;

  if (mergedRanges.length > 1)
    return mergedRanges[0][1] + 1;
  return minX - 1;
}

void main ()
{ 
  Sensor[] sensors = parseInput();

  foreach (y; MinY..MaxY)
    {
      int x = scanRow(sensors, y, MinX, MaxX);
      if (x >= MinX)
        {
          writeln(x * 4000000L + y);
          break;
        }
    }
}
