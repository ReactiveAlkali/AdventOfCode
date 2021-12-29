#include <iostream>
#include <cstdio>
#include <array>
#include <set>
#include <list>
#include <string>
#include <map>
#include <iterator>
#include <algorithm>
#include <functional>
#include <cmath>
#include <vector>

using fingerprint_map_t =
  std::map<std::array<int,4>,std::set<std::array<int,3>>>;
using beacon_pair_t = std::array<std::array<int,4>,2>;

// Beacon coords have an extra element with value 1, this is to make applying
// transformations to beacons easier.
struct Scanner
{
  std::set<std::array<int,4>> beacons {};
  std::set<std::array<int,3>> scanners {};
  fingerprint_map_t fingerprints {};
};
  
//
// Generates a unique fingerprint between a pair of two points
//
std::array<int,3>
generate_fingerprint (const std::array<int,4>& p1, const std::array<int,4>& p2)
{
  int dx { p1[0] - p2[0] };
  int dy { p1[1] - p2[1] };
  int dz { p1[2] - p2[2] };
  int dxdx { dx * dx };
  int dydy { dy * dy };
  int dzdz { dz * dz };
  
  // The unique fingerprint is composed of the distance squared between
  // the two points, the largest of the three square differences, and
  // the smallest of the three square differences.
  std::array<int,3> fingerprint {};
  fingerprint[0] = dxdx + dydy + dzdz;
  fingerprint[1] = std::max({ dxdx, dydy, dzdz });
  fingerprint[2] = std::min({ dxdx, dydy, dzdz });

  return fingerprint;
}

//
// Generates a set of fingerprints for each beacon in the given scanner
//
void
generate_fingerprints (Scanner& scanner)
{
  for (const std::array<int,4>& beacon0 : scanner.beacons)
    {
      std::set<std::array<int,3>> fingerprints {};

      for (const std::array<int,4>& beacon1 : scanner.beacons)
        {
          if (beacon0 != beacon1)
            fingerprints.insert(generate_fingerprint(beacon0, beacon1));
        }
  
      scanner.fingerprints[beacon0] = fingerprints;
    }
}

//
// Parses our input into a list of scanner reports
//
std::list<Scanner>
parse_input ()
{
  std::list<Scanner> scanners;
  std::string input_line {};

  // The first getline is to get rid of the line containing
  // "--- scanner # ---".  The inner loop is what actually gets the data
  // values.
  while (std::getline(std::cin, input_line))
    {
      Scanner scanner {};
      
      while (std::getline(std::cin, input_line) && !input_line.empty())
        {
          std::array<int,4> beacon { 0, 0, 0, 1 };
          std::sscanf(input_line.c_str(), "%d,%d,%d", &beacon[0], &beacon[1],
                      &beacon[2]);
          scanner.beacons.insert(beacon);
        }      
      generate_fingerprints(scanner);
      scanner.scanners.insert(std::array<int,3>{});
      scanners.push_back(scanner);
    }

  return scanners;
}

//
// Finds overlapping beacons between the two given scanners
//
std::vector<beacon_pair_t>
scanner_overlap (Scanner& s0, Scanner& s1)
{
  std::vector<beacon_pair_t> beacon_pairs {};
  std::set<std::array<int,4>>::iterator it0 { s0.beacons.begin() };

  // In order to compute an isometric transformation matrix we require four
  // corresponding beacon pairs, any more is unneccessary.
  while (it0 != s0.beacons.end() && beacon_pairs.size() < 4)
    {
      std::set<std::array<int,3>>& b0_fingerprints { s0.fingerprints[*it0] };
      std::set<std::array<int,4>>::iterator it1 { s1.beacons.begin() };
      
      bool found_matching { false };  
      while (it1 != s1.beacons.end() && !found_matching)
        {
          std::set<std::array<int,3>>& b1_fingerprints { s1.fingerprints[*it1] };
          std::set<std::array<int,3>> intersection {};

          std::set_intersection(b0_fingerprints.begin(), b0_fingerprints.end(),
                                b1_fingerprints.begin(), b1_fingerprints.end(),
                                std::inserter(intersection, intersection.end()));
          // 12 matching beacons is what's required for there to be overlap.
          // This means the current beacon plus fingerprints with 11 other
          // beacons.
          if (intersection.size() > 10)
            {
              found_matching = true;
              beacon_pairs.push_back(beacon_pair_t{*it0, *it1});
            }
          
          ++it1;
        }
      ++it0;
    }

  return beacon_pairs;
}

//
// Generate a transformation matrix given a list of beacon pairs
//
std::array<std::array<int,4>,4>
generate_transformation (std::vector<beacon_pair_t> beacon_pairs)
{
  std::array<std::array<int,4>,4> transformation {};
  // fourth row is always the same
  transformation[3] = std::array<int,4>{ 0, 0, 0, 1 };

  // We will solve an augmented matrix to find the three other rows of the
  // transformation.
  std::array<std::array<double,7>,4> augmented_matrix {};
  for (int i {}; i < 4; ++i)
    {
      for (int j {}; j < 3; ++j)
        {
          augmented_matrix[i][j] = beacon_pairs[i][1][j];
          augmented_matrix[i][j + 4] = beacon_pairs[i][0][j];
        }
      augmented_matrix[i][3] = 1;
    }

  // Use Guass-Jordan elimination on augmented matrix
  for (int j { 0 }; j < 4; ++j)
    {
      for (int i { 0 }; i < 4; ++i)
        {
          if (i != j)
            {
              double b { augmented_matrix[i][j] / augmented_matrix[j][j] };
              for (int k { 0 }; k < 7; ++k)
                {
                  augmented_matrix[i][k] =
                    augmented_matrix[i][k] - b * augmented_matrix[j][k];
                }
            }
        }
    }

  // extract solution to our transformation matrix
  for (int i {}; i < 3; ++i)
    {
      for (int j{}; j < 4; ++j)
        {
          transformation[i][j] = std::round(augmented_matrix[j][i+4]
                                            / augmented_matrix[j][j]);
        }
    }

  return transformation;
}

Scanner
assemble_beacon_map (std::list<Scanner>& scanners)
{
  Scanner full_map { scanners.front() };
  scanners.pop_front();

  while (!scanners.empty())
    {
      std::list<Scanner>::iterator it { scanners.begin() };
      while (it != scanners.end())
        {
          auto beacon_pairs { scanner_overlap(full_map, *it) };

          // 4 beacon pairs is what we need to generate our transformation
          if (beacon_pairs.size() == 4)
            {
              auto transformation { generate_transformation(beacon_pairs) };

              for (const std::array<int,4>& beacon : (*it).beacons)
                {
                  // Multiply the beacon by our transformation to get the
                  // beacon's coordinates relative to scanner 0
                  std::array<int,4> transformed {};
                  for (int i {}; i < 4; ++i)
                    {
                      for (int j {}; j < 4; ++j)
                        transformed[i] += transformation[i][j] * beacon[j];
                    }
                  full_map.beacons.insert(transformed);
                }
              generate_fingerprints(full_map);
              full_map.scanners.insert(std::array<int,3>{
                  transformation[0][3], transformation[1][3],
                  transformation[2][3] });
              it = scanners.erase(it);
            }
          else
            ++it;
        }
    }
  return full_map;
}

//
// Finds the largest Manhatten distance between any two scanners
//
int
max_manhatten_distance (const std::set<std::array<int,3>>& scanners)
{
  int max_distance {};
  auto it0 { scanners.begin() };

  while (it0 != scanners.end())
    {
      auto it1 { it0 };
      ++it1;
      while (it1 != scanners.end())
        {
          int distance {};
          for (int i = 0; i < 3; ++i)
            distance += std::abs((*it0)[i] - (*it1)[i]);
          if (distance > max_distance)
            max_distance = distance;
          ++it1;
        }
      ++it0;
    }

  return max_distance;
}

int
main ()
{
  std::list<Scanner> scanners { parse_input() };
  Scanner full_map { assemble_beacon_map(scanners) };
  std::cout << "# of beacons: " << full_map.beacons.size() << '\n';

  int max_distance { max_manhatten_distance(full_map.scanners) };
  std::cout << "Largest Manhatten distance: " << max_distance << '\n';
  
  return 0;
}
