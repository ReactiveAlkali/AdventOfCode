#include <cstdio>
#include <array>
#include <sstream>
#include <algorithm>

struct Player
{
  static int winning_score;
  int score {};
  int position {};

  bool
  move (int spaces)
  {
    // Move the specified number of spaces and handle wrapping
    position += spaces;
    position %= 10;
    if (position == 0)
      position = 10;

    score += position;
    return score >= winning_score;
  }
};

struct Die
{
  int times_rolled {};
  int value {};

  int
  roll (int times)
  {
    int sum {};
    for (int i {}; i < times; ++i)
      {
        ++value;
        if (value > 100)
          value = 1;
        sum += value;
      }
    times_rolled += times;
    return sum;
  }
};

std::array<Player,2>
parse_input ()
{
  int start1{};
  int start2{};

  std::scanf("Player 1 starting position: %d\n", &start1);
  std::scanf("Player 2 starting position: %d\n", &start2);

  return std::array<Player,2>{ Player{ 0, start1 }, Player{ 0, start2 } };
}


void
deterministic_game (std::array<Player,2>& players)
{
  Die die {};
  bool won { false };
  bool turn { false };
  
  while (!won)
    {
      won = players[turn].move(die.roll(3));
      turn = !turn;
    }

  std::printf("%d\n", die.times_rolled * players[turn].score);
}

//
// Gets the frequency in which three rolls results in the given number of
// spaces.
//
int
get_frequency (int spaces)
{
  int freq {};
  switch (spaces)
    {
    case 3: freq = 1; break;
    case 4: freq = 3; break;
    case 5: freq = 6; break;
    case 6: freq = 7; break;
    case 7: freq = 6; break;
    case 8: freq = 3; break;
    case 9: freq = 1;
    }
  return freq;
}

std::array<unsigned long long,2>
rec_quantum_game (std::array<Player,2> players, int spaces, bool turn)
{
  std::array<unsigned long long,2> wins {};

  bool won = players[turn].move(spaces);
  if (won)
    ++wins[turn];
  else
    {
      for (int i { 3 }; i <= 9; ++i)
        {
          std::array<unsigned long long,2> tmp {
            rec_quantum_game(players, i, !turn) };
          wins[0] += tmp[0] * get_frequency(i);
          wins[1] += tmp[1] * get_frequency(i);
        }
    }
  
  return wins;
}

unsigned long long
quantum_game (std::array<Player,2> players)
{
  unsigned long long player1_wins {};
  unsigned long long player2_wins {};

  // there are only 7 possible values three dice rolls can have
  for (int i { 3 }; i <= 9; ++i)
    {
      std::array<unsigned long long,2> temp {
        rec_quantum_game(players, i, false) };
      player1_wins += temp[0] * get_frequency(i);
      player2_wins += temp[1] * get_frequency(i);
    }

  return std::max(player1_wins, player2_wins);
}

int Player::winning_score = 0;

int
main (int argc, char** argv)
{
  if (argc <= 1)
    return 1;
  
  std::array<Player,2> players { parse_input() };

  // Run the specified part of the problem
  std::string part { argv[1] };
  if (part == "1")
    {
      Player::winning_score = 1000;
      deterministic_game(players);
    }
  else if (part == "2")
    {
      Player::winning_score = 21;
      unsigned long long universes { quantum_game(players) };
      std::printf("%lld\n", universes);
    }
  
  return 0;
}
