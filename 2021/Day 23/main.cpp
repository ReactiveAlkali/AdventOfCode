#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <tuple>

const std::string PART1_GOAL_STATE { "...........ABCDABCD" };
const std::string PART2_GOAL_STATE { "...........ABCDABCDABCDABCD" };

auto compare_func = [](const std::pair<std::string,int>& lhs,
                       const std::pair<std::string,int>& rhs)
 { return lhs.second > rhs.second; };

using state_queue_t =
  std::priority_queue<std::pair<std::string,int>,
                      std::vector<std::pair<std::string,int>>,
                      decltype(compare_func)>;

//
// Parses the given input into a string representing the initial state
//
std::string
parse_input ()
{
  std::string init_state {};
  std::string input_line {};

  while (std::cin >> input_line)
    {
      for (char c : input_line)
        {
          if (c != '#')
            init_state.push_back(c);
        }
    }

  return init_state;
}

//
// The cost of each step of the given amphipod
//
int
step_cost (char amphipod)
{
  int cost {};
  switch (amphipod)
    {
    case 'A': cost = 1; break;
    case 'B': cost = 10; break;
    case 'C': cost = 100; break;
    case 'D': cost = 1000; break;
    }
  return cost;
}

//
// Checks whether the given hallway positions is above a room
//
bool
above_room (int hallway_position)
{
  bool is_above_room { false };
  // is it bad to hard code numbers, probably but it's the easy way
  switch (hallway_position)
    {
    case 2: is_above_room = true; break;
    case 4: is_above_room = true; break;
    case 6: is_above_room = true; break;
    case 8: is_above_room = true; break;
    }
  return is_above_room;
}

//
// Gets the hallway positions that are accessible from the given room and the
// number of steps needed to reach that position form the top of the room.
//
std::vector<std::pair<int,int>>
accessible_hallway_positions (const std::string& state, int room)
{
  std::vector<std::pair<int,int>> accessible {};
  int position_above_room { 2 + 2 * room };

  for (int i { position_above_room }; i < 11 && state[i] == '.'; ++i)
    {
      if (!above_room(i))
        accessible.push_back(std::make_pair(i, i - position_above_room + 1));
    }
  for (int i { position_above_room }; i >= 0 && state[i] == '.'; --i)
    {
      if (!above_room(i))
        accessible.push_back(std::make_pair(i, position_above_room - i + 1));
    }
  
  return accessible;
}

//
// Checks the state of the given room.  Returns a tuple of three elements each
// having the following meaning respectively:
// - Whether the room is empty or contains only the correct members
// - The index of the top amphipod of the room
// - The depth of the top amphipod in the room
//
std::tuple<bool,int,int>
check_room (const std::string& state, int room)
{
  bool is_clean { true };
  int top_element { -1 };
  int depth { -1 };

  for (int i { room + 11 }; i < state.size(); i += 4)
    {
      if (top_element < 0)
        {
          ++depth;
          if (state[i] != '.')
            top_element = i;
        }
      if (is_clean)
        is_clean = state[i] == '.' || state[i] == (room + 'A');
    }

  return std::make_tuple(is_clean, top_element, depth);
}

//
// Check whether there is a clear path between the given hallway postion and
// the given room.  Returns the number of steps in said path or -1 if the room
// is unaccessible.
//
int
room_accessible (const std::string& state, int hallway_pos, int room)
{
  int steps { -1 };
  bool accessible { true };
  int pos_above_room { 2 + 2 * room };

  for (int i { pos_above_room }; i > hallway_pos && accessible; --i)
    accessible = state[i] == '.';
  for (int i { pos_above_room }; i < hallway_pos && accessible; ++i)
    accessible = state[i] == '.';

  if (accessible)
    steps = std::abs(pos_above_room - hallway_pos) + 1;
  
  return steps;
}

//
// Add all possible moves in the current state to the priority queue
//
void
add_moves (state_queue_t& pq, const std::string& state, int cost)
{
  std::unordered_map<int,std::tuple<bool,int,int>> room_states {};
  
  // move from room to hallway
  for (int room {0}; room < 4; ++room)
    {
      bool clean;
      int top;
      int depth;
      room_states[room] = check_room(state, room);
      std::tie(clean, top, depth) = room_states[room];

      auto moves { accessible_hallway_positions(state, room) };

      // Ignore this room if it's clean
      if (!clean)
        {
          for (auto& move : moves)
            {
              int move_cost { (depth + move.second) * step_cost(state[top]) };
              std::string tmp { state };
              std::swap(tmp[top], tmp[move.first]);

              pq.push(std::make_pair(tmp, move_cost + cost));
            }
        }
    }
  // move from hallway to room
  for (int i { 0 }; i < 11; ++i)
    {
      if (state[i] != '.')
        {
          int room { state[i] - 'A' };
          int steps_to_room { room_accessible(state, i, room) };
          bool clean;
          int top;
          int depth;
          std::tie(clean, top, depth) = room_states[room];
          
          if (clean && steps_to_room > 0)
            {
              std::string tmp { state };
              int move_cost {};
              int room_index {};
              if (top < 0)
                {
                  room_index = room + 11 + 4 * depth;
                  move_cost = (steps_to_room + depth) * step_cost(state[i]);
                }
              else
                {
                  // if there is a top element then the first open space will be
                  // above it
                  room_index = room + 11 + 4 * (depth - 1);
                  move_cost = (steps_to_room + depth - 1) * step_cost(state[i]);
                }
              std::swap(tmp[room_index], tmp[i]);
              pq.push(std::make_pair(tmp, move_cost + cost));
            }
        }
    }
  
}

//
// Determines the minimum cost to obtain the given goal state from the initial
// state.
//
int
minimum_energy (const std::string& init_state, const std::string& goal_state)
{
  std::unordered_map<std::string,int> final_costs {};
  state_queue_t pq(compare_func);

  pq.push(std::make_pair(init_state,0));

  // keep going till we reach the goal state
  while (final_costs.count(goal_state) == 0 && !pq.empty())
    {
      auto curr_state_pair { pq.top() };
      pq.pop();

      // ignore this state if we've already been here
      if (final_costs.count(curr_state_pair.first) == 0)
        {
          //std::cout << curr_state_pair.first << ' ' << curr_state_pair.second << '\n';
          final_costs[curr_state_pair.first] = curr_state_pair.second;
          add_moves(pq, curr_state_pair.first, curr_state_pair.second);
        }
    }

  return final_costs[goal_state];
}

int
main (int argc, char** argv)
{ 
  std::string init_state { parse_input() };

  int min_cost { minimum_energy(init_state, PART2_GOAL_STATE) };
  std::cout << min_cost << '\n';
  return 0;
}
