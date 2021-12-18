#include <iostream>
#include <list>
#include <stack>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>

std::list<std::string>
parse_input ()
{
  std::list<std::string> lines {};
  std::string line {};

  while (std::cin >> line)
    lines.push_back(line);

  return lines;
}

//----------------------------------------------------
// Gets to syntax error points of the given character
//----------------------------------------------------
int
get_points (char c)
{
  int points {};
  switch (c)
    {
    case ')': points = 3; break;
    case ']': points = 57; break;
    case '}': points = 1197; break;
    case '>': points = 25137; break;
    }
  return points;
}

//------------------------------------------------------------
// Checks whether the given character is an opening character
//------------------------------------------------------------
bool
is_opening (char c)
{
  bool opening {};
  switch (c)
    {
    case '(':
    case '[':
    case '{':
    case '<':
      opening = true;
      break;
    default:
      opening = false;
    }
  return opening;
}

//--------------------------------------------------------------------------
// Determines whether the given opening and closing characters are matching
//--------------------------------------------------------------------------
bool
matching (char opening, char closing)
{
  return (opening == '(' && closing == ')') || (opening == '[' && closing == ']')
    || (opening == '{' && closing == '}') || (opening == '<' && closing == '>');
}

//-------------------------------------------------------------
// Discards all corrupt lines from the list
// Returns the total syntax error score of the discarded lines
//-------------------------------------------------------------
int
discard_corrupt_lines (std::list<std::string>& lines)
{
  int syntax_error_score {};
  std::list<std::string>::iterator it = lines.begin();

  // check whether each line is corrupt
  while (it != lines.end())
    {
      std::stack<char> opening_characters {};
      bool corrupt { false };
      std::string line { *it };

      // check whether the current line is corrupt
      for (int i = 0; i < line.size() && !corrupt; ++i)
        {
          if (is_opening(line[i]))
            opening_characters.push(line[i]);
          else if (matching(opening_characters.top(), line[i]))
            opening_characters.pop();
          else
            {
              corrupt = true;
              syntax_error_score += get_points(line[i]);
            }
        }

      // increment our iterator
      if (corrupt)
        it = lines.erase(it);
      else
        ++it;
    }

  return syntax_error_score;
}

//-----------------------------------------------------------------------------
// Returns the corresponding closing character for the given opening character
//-----------------------------------------------------------------------------
char
close (char c)
{
  char closing_character {};
  switch (c)
    {
    case '(': closing_character = ')'; break;
    case '[': closing_character = ']'; break;
    case '{': closing_character = '}'; break;
    case '<': closing_character = '>'; break;
    }
  return closing_character;
}

//--------------------------------------------------------------------
// Finds the sequence of closing characters which completes each line
// Returns a list of the closing character sequences
//--------------------------------------------------------------------
std::list<std::string>
complete_lines (const std::list<std::string>& lines)
{
  std::list<std::string> completion_strings {};

  for (const std::string& line : lines)
    {
      std::stack<char> opening_characters {};

      // get all the open characters for our incomplete line
      for (char c : line)
        {
          if (is_opening(c))
            opening_characters.push(c);
          else
            opening_characters.pop();
        }

      // find the completion string of the current line
      std::string closing_characters {};
      while (!opening_characters.empty())
        {
          char curr = opening_characters.top();
          closing_characters.push_back(close(curr));
          opening_characters.pop();
        }

      completion_strings.push_back(closing_characters);
    }

  return completion_strings;
}

//-----------------------------------------------------
// Calculates the score of the given completion string
//-----------------------------------------------------
long
completion_str_score (const std::string& str)
{
  long score {};
  for (char c : str)
    {
      score *= 5;
      switch (c)
        {
        case ')': ++score; break;
        case ']': score += 2; break;
        case '}': score += 3; break;
        case '>': score += 4; break;
        }
    }
  return score;
}

int
main ()
{
  std::list<std::string> lines { parse_input() };

  // Part 1
  int syntax_error_score { discard_corrupt_lines(lines) };
  std::cout << "Syntax error score of corrupted lines: " << syntax_error_score
            << '\n';

  // Part 2
  std::list<std::string> completion_strings { complete_lines(lines) };

  // get the list of all completion scores
  std::vector<long> completion_scores {};
  for (const std::string& str : completion_strings)
    completion_scores.push_back(completion_str_score(str));

  // get the middle score
  std::sort(completion_scores.begin(), completion_scores.end());
  long middle_score { completion_scores[completion_scores.size() / 2] };
  std::cout << "Middle completion score: " << middle_score << '\n';

  return 0;
}
