#include "BingoCard.hpp"
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <limits>
#include <iterator>

BingoCard
read_card ()
{
  std::array<std::array<int, 5>, 5> temp{};
  for (auto& row : temp)
    {
      for (int& num : row)
        std::cin >> num;
    }
  // remove trailing newlines
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  return BingoCard(temp);
}

std::list<BingoCard>::iterator
mark_cards (std::list<BingoCard>& cards, int num)
{
  std::list<BingoCard>::iterator it = cards.begin();
  bool won = false;
  while (it != cards.end() && !won)
    {
      won = (*it).mark_card(num);
      it++;
    }
  if (won)
    it--;
  return it;
}

int
main()
{
  std::list<int> call_order;
  std::list<BingoCard> cards;

  // get our call order
  std::string input{};
  std::cin >> input;
  std::stringstream ss(input);
  while (ss.good())
    {
      std::string substr;
      std::getline(ss, substr, ',');
      call_order.push_back(stoi(substr));
    }
  
  // get our cards
  while (std::cin.peek() != EOF)
    {
      cards.push_back(read_card());
    }

  std::list<BingoCard>::iterator winning_card = cards.end();
  std::list<int>::iterator it = call_order.begin();

  while (winning_card == cards.end() && it != call_order.end())
    {
      winning_card = mark_cards(cards, (*it));

      //remove winning cards until there is only one card left
      if (winning_card != cards.end() && cards.size() > 1)
        {
          cards.erase(winning_card);
          winning_card = cards.end();
        }
      else
        it++;
    }
  it--;

  std::cout << (*winning_card).unmarked_sum() * (*it) << '\n';

  return 0;
}
