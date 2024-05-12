import std.algorithm;
import std.stdio;
import std.string;

string addSnafu(string lhs, string rhs)
{
  import std.conv;
  auto additionMatrix =
    ['2': ['2': "1-", '1': "1=", '0': "2", '-': "1", '=': "0"],
     '1': ['2': "1=", '1': "2", '0': "1", '-': "0", '=': "-"],
     '0': ['2': "2", '1': "1", '0': "0", '-': "-", '=': "="],
     '-': ['2': "1", '1': "0", '0': "-", '-': "=", '=': "-2"],
     '=': ['2': "0", '1': "-", '0': "=", '-': "-2", '=': "-1"]];
  
  // Which number has more digits
  string longSnafu;
  string shortSnafu;
  if (lhs.length > rhs.length)
    {
      longSnafu = lhs;
      shortSnafu = rhs;
    }
  else
    {
      longSnafu = rhs;
      shortSnafu = lhs;
    }
  longSnafu = longSnafu.dup.reverse();
  shortSnafu = shortSnafu.dup.reverse();

  char[] sum;
  string carry;
  foreach (i, digit; longSnafu)
    {
      char[] result = (i < shortSnafu.length
		       ? additionMatrix[digit][shortSnafu[i]]
		       : digit.to!string).dup.reverse();
      
      if (carry != "")
	{
	  string temp = additionMatrix[result[0]][carry[0]].dup.reverse();
	  result[0] = temp[0];
	  if (temp.length > 1)
	    {
	      if (result.length > 1)
		result[1] = additionMatrix[temp[1]][result[1]][0];
	      else
		result ~= temp[1];
	    }
	  carry = "";
	}

      sum ~= result[0];
      if (result.length > 1)
	carry = to!string(result[1]);      
    }

  if (carry != "")
    sum ~= carry;

  return sum.reverse().dup;
}

void main()
{
  string line;
  string sum = "0";
  while ((line = readln().strip) !is null)
    sum = addSnafu(sum, line);

  writeln(sum);
}
