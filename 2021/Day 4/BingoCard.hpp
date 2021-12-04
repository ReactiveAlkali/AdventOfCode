#ifndef BINGO_CARD_H
#define BINGO_CARD_H

#include <array>
#include <iostream>

class BingoCard
{
  std::array<std::array<int, 5>, 5> m_card{};
  std::array<std::array<bool, 5>, 5> m_marked{};

  // Check whether all numbers in a given row or column are marked
  bool check_row (int row);
  bool check_col (int col);

public:
  BingoCard (std::array<std::array<int, 5>, 5> card);

  // Marks the given number on the card if it exists
  // Returns true if the card won false otherwise
  bool mark_card (int num);

  void print_card ();

  int unmarked_sum();
};

#endif
