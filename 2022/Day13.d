import std.json;
import std.stdio;
import std.string : strip;
import std.typecons;

JSONValue[2][] readInput ()
{
  JSONValue[2][] packetPairs;
  JSONValue[2] pair;
  int index;

  string line;
  while ((line = readln.strip) !is null)
    {
      if (line == "")
        {
          packetPairs ~= pair;
          index = 0;
        }
      else
        pair[index++] = parseJSON(line);
    }
  packetPairs ~= pair;
  return packetPairs;
}

int compareValues (JSONValue left, JSONValue right)
{
  int result;
  
  if (left.type() == JSONType.integer && right.type() == JSONType.integer)
    result = cast(int)(left.integer - right.integer);
  else if (left.type() == JSONType.array && right.type() == JSONType.array)
    {
      auto leftList = left.array;
      auto rightList = right.array;
      for (int i = 0; i < leftList.length && i < rightList.length && !result; ++i)
        result = compareValues(leftList[i], rightList[i]);

      if (!result)
        result = cast(int)(leftList.length - rightList.length);
    }
  else if (left.type() == JSONType.integer && right.type == JSONType.array)
    {
      JSONValue leftList;
      leftList.array = [left];
      result = compareValues(leftList, right);
    }
  else if (left.type == JSONType.array && right.type == JSONType.integer)
    {
      JSONValue rightList;
      rightList.array = [right];
      result = compareValues(left, rightList);
    }
  return result;
}

JSONValue[] flatten (JSONValue[2][] pairs)
{
  JSONValue[] packets;
  foreach (pair; pairs)
    {
      packets ~= pair[0];
      packets ~= pair[1];
    }
  return packets;
}

void main ()
{
  JSONValue[2][] packetPairs = readInput();

  // part 1
  int indexSum;
  foreach (i, pair; packetPairs)
    {
      int result = compareValues(pair[0], pair[1]);
      if (result <= 0)
        indexSum += i + 1;
    }
  writeln(indexSum);

  // part 2
  import std.algorithm;
  JSONValue[] packets = flatten(packetPairs);
  packets ~= JSONValue([[2]]);
  packets ~= JSONValue([[6]]);
  packets.sort!((a, b) => compareValues(a, b) < 0);

  long result = 1;
  foreach (i, packet; packets)
    {
      if (packet == JSONValue([[2]]))
        result *= i + 1;
      if (packet == JSONValue([[6]]))
        result *= i + 1;
    }
  writeln(result);
}
