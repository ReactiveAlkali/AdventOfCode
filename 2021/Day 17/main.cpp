#include <cstdio>
#include <cmath>
#include <array>
#include <limits>
#include <iostream>

//
// Parses the input to get the bounds of the target area
//
std::array<int,4>
parse_input ()
{
  std::array<int,4> target_bounds {};
  std::scanf("target area: x=%d..%d, y=%d..%d\n", &target_bounds[0],
             &target_bounds[1], &target_bounds[2], &target_bounds[3]);
  return target_bounds;
}

//
// Performs a single step updating the given position and volocity of
// the probe
//
void
step (int& x, int& y, int& vx, int& vy)
{
  x += vx;
  y += vy;
  if (vx > 0)
    --vx;
  --vy;
}

//
// Determines whether the probe is within the target area
//
bool
in_target (const std::array<int,4>& target, int x, int y)
{
  return x >= target[0] && x <= target[1] && y >= target[2] && y <= target[3];
}

//
// Determine whether we have missed the target
//
bool
missed_target (const std::array<int,4>& target, int x, int y)
{
  return x > target[1] || y < target[2];
}

//
// Determine the initial velocity that maximizes the initial y velocity such
// that the probe still eventually ends up in the target area.
//
int
find_max_init_vy (std::array<int,4> target, int min_vx)
{
  int max_vy { std::numeric_limits<int>::min() };

  // The initial x velocity can't be greater than the outer x bound of the
  // target, otherwise it will immediately overshoot.
  for (int init_vx { min_vx }; init_vx <= target[1]; ++init_vx)
    {
      // The same as above also applies to the y velocity.  Since we don't know
      // what the max y velocity will be I've just choosen a large number.
      for (int init_vy { target[2] }; init_vy < 1000; ++init_vy)
        {
          int x {};
          int y {};
          int vx { init_vx };
          int vy { init_vy };

          while (!missed_target(target, x, y) && !in_target(target, x, y))
            {
              step(x, y, vx, vy);
              if (in_target(target, x, y) && init_vy > max_vy)
                max_vy = init_vy;
            }
        }
    }

  return max_vy;
}

//
// Determines the highest y value the probe reaches with the given initial
// velocity.
//
int
find_highest_y (int max_vy)
{
  int max_y { std::numeric_limits<int>::min() };
  int x {};
  int y {};
  int vy { max_vy };
  int vx {};

  while (y > max_y)
    {
      max_y = y;
      step(x, y, vx, vy);
    }

  return max_y;
}

//
// Count how many distinct velocity values cause the probe to be in the target
// area after any step.
//
int
count_init_velocities(const std::array<int,4>& target_bounds, int min_init_vx,
                      int max_init_vx, int min_init_vy, int max_init_vy)
{
  int count {};

  for (int init_vx { min_init_vx }; init_vx <= max_init_vx; ++init_vx)
    {
      for (int init_vy { min_init_vy }; init_vy <= max_init_vy; ++init_vy)
        {
          int x {};
          int y {};
          int vx { init_vx };
          int vy { init_vy };

          while (!missed_target(target_bounds, x, y) && !in_target(target_bounds, x, y))
            {
              step(x, y, vx, vy);
              if (in_target(target_bounds, x, y))
                ++count;
            }
        }
    }

  return count;
}

int
main ()
{
  std::array<int,4> target_bounds { parse_input() };
  
  // At minimum the sum of the x velocities need to be greater than the lower
  // bound of the target area.
  int min_init_vx { static_cast<int>((-1 + std::sqrt(1 + 8 * target_bounds[0]))
                                     / 2) };
  int max_init_vy { find_max_init_vy(target_bounds, min_init_vx) };

  std::cout << "Highest y position: " << find_highest_y(max_init_vy) << '\n';

  int num_init_velocities { count_init_velocities(target_bounds, min_init_vx,
                                                  target_bounds[1],
                                                  target_bounds[2],
                                                  max_init_vy) };
  std::cout << "Velocity values: " << num_init_velocities << '\n';
  
  return 0;
}
