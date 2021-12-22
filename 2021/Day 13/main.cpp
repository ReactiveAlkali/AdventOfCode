#include <iostream>
#include <list>
#include <set>
#include <utility>
#include <cstdio>
#include <iterator>
#include <vector>

using dot_t = std::pair<int,int>;
using fold_t = std::pair<char,int>;
using paper_t = std::vector<std::vector<char>>;

//-----------------------------------------
// Reads a set of dots from standard input
//-----------------------------------------
std::set<dot_t>
get_dots ()
{
  std::set<dot_t> dots {};
  dot_t temp {};

  while (std::scanf("%d,%d", &temp.first, &temp.second))
    dots.insert(temp);

  return dots;
}

//-------------------------------------------
// Reads a list of folds from standard input
//-------------------------------------------
std::list<fold_t>
get_folds ()
{
  std::list<fold_t> folds {};
  fold_t temp {};

  while (std::scanf("fold along %c=%d\n", &temp.first, &temp.second) != EOF)
    folds.push_back(temp);

  return folds;
}

//--------------------------------------------------------------------
// Changes dot positions corresponding the the given fold instruction
//--------------------------------------------------------------------
void
fold_paper (std::set<dot_t>& dots, fold_t& fold)
{
  std::set<dot_t>::iterator it { dots.begin() };

  // iterate through all our dots
  while (it != dots.end())
    {
      // get the current dot and increment our iterator
      dot_t dot = *it;
      ++it;

      switch (fold.first)
        {
        case 'x':
          // fold the paper left
          if (dot.first > fold.second)
            {
              dots.erase(dot);
              dot.first = fold.second * 2 - dot.first;
              dots.insert(dot);
            }
          break;
        case 'y':
          // fold the paper up
          if (dot.second > fold.second)
            {
              dots.erase(dot);
              dot.second = fold.second * 2 - dot.second;
              dots.insert(dot);
            }
          break;
        }
    }
}

//------------------------------------------------------------------
// Expands the number of columns contained in each row of the array
//------------------------------------------------------------------
void
expand_col (paper_t& paper, std::size_t col_num)
{
  for (std::vector<char>& row : paper)
    row.resize(col_num, ' ');
}

//--------------------------------------------------
// Prints the pattern represented by the given dots
//--------------------------------------------------
void
print_paper (std::set<dot_t>& dots)
{
  paper_t paper(1, std::vector<char>(1, ' '));

  // Construct a vector representation of the paper with the given dots
  for (const dot_t& dot : dots)
    {
      // increase the paper size if neccessary to fit the dot
      if (dot.first >= paper[0].size())
        expand_col(paper, dot.first + 1);
      if (dot.second >= paper.size())
        paper.resize(dot.second + 1, std::vector<char>(paper[0].size(), ' '));

      paper[dot.second][dot.first] = '#';
    }

  // print the paper
  for (std::vector<char>& row : paper)
    {
      for (char c : row)
        std::cout << c;
      std::cout << '\n';
    }
}

int
main ()
{
  std::set<dot_t> dots { get_dots() };
  std::list<fold_t> folds { get_folds() };

  for (fold_t& fold : folds)
    fold_paper(dots, fold);

  print_paper(dots);
  
  return 0;
}
