#include "BingoCard.hpp"

BingoCard::BingoCard (std::array<std::array<int, 5>, 5> card)
  : m_card{ card }
 {
 }

void
BingoCard::print_card ()
{
  for (int i = 0; i < m_card.size(); ++i)
    {
      for (int j = 0; j < m_card[i].size(); ++j)
        {
          if (m_marked[i][j])
            std::cout << "\e[1m" << m_card[i][j] << "\e[0m";
          else
            std::cout << m_card[i][j];
          std::cout << '\t';
        }
      std::cout << '\n';
    }
}

bool
BingoCard::mark_card (int num)
{
  bool found{ false };
  bool won{ false };

  // Search for the given num and mark it if found
  for (int i = 0; i < m_card.size() && !found; ++i)
    {
      for (int j = 0; j < m_card[0].size() && !found; ++j)
        {
          if (m_card[i][j] == num)
            {
              m_marked[i][j] = true;
              found = true;
              won = check_row(i) || check_col(j);
            }
        }
    }

  return won;
}

bool
BingoCard::check_row (int row)
{
  bool all_marked{ true };
  
  for (int i = 0; i < m_marked[row].size() && all_marked; ++i)
    all_marked = m_marked[row][i];

  return all_marked;
}

bool
BingoCard::check_col (int col)
{
  bool all_marked{ true };

  for (int i = 0; i < m_marked.size() && all_marked; ++i)
    all_marked = m_marked[i][col];

  return all_marked;
}

int
BingoCard::unmarked_sum ()
{
  int sum = 0;
  for (int i = 0; i < m_card.size(); ++i)
    {
      for (int j = 0; j < m_card[i].size(); ++j)
        {
          if (!m_marked[i][j])
            sum += m_card[i][j];
        }
    }
  return sum;
}
