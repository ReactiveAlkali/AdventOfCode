#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <limits>

//----------------------------------------------------
// Converts te given hex character to a binary string
//----------------------------------------------------
std::string
hex_char_to_binary (char c)
{
  std::string binary {};
  switch (c)
    {
    case '0': binary = "0000"; break;
    case '1': binary = "0001"; break;
    case '2': binary = "0010"; break;
    case '3': binary = "0011"; break;
    case '4': binary = "0100"; break;
    case '5': binary = "0101"; break;
    case '6': binary = "0110"; break;
    case '7': binary = "0111"; break;
    case '8': binary = "1000"; break;
    case '9': binary = "1001"; break;
    case 'A': binary = "1010"; break;
    case 'B': binary = "1011"; break;
    case 'C': binary = "1100"; break;
    case 'D': binary = "1101"; break;
    case 'E': binary = "1110"; break;
    case 'F': binary = "1111"; break;
    }
  return binary;
}

//------------------------------------------------------
// Reads the hexidecimal input string into a bit array
//------------------------------------------------------
std::vector<bool>
read_input ()
{
  // Get our hex string
  std::string hex {};
  std::cin >> hex;

  // Convert our hex string to a binary string
  std::string binary {};
  for (char c : hex)
    binary.append(hex_char_to_binary(c));

  // convert our binary string to bits
  std::vector<bool> packet {};
  for (char c : binary)
    packet.push_back(c == '1');
  std::reverse(packet.begin(), packet.end());

  return packet;
}

//---------------------------------------------------------------------
// Extracts bits from the end of the given bool array and returns them
// as a bitset.
//---------------------------------------------------------------------

std::bitset<15>
extract_15_bits (std::vector<bool>& packet)
{
  std::bitset<15> binary {};

  for (int i { 0 }; i < 15; ++i)
    {
      binary <<= 1;
      binary.set(0, packet.back());
      packet.pop_back();
    }

  return binary;
}

std::bitset<3>
extract_3_bits (std::vector<bool>& packet)
{
  std::bitset<3> binary {};

  for (int i { 0 }; i < 3; ++i)
    {
      binary <<= 1;
      binary.set(0, packet.back());
      packet.pop_back();
    }

  return binary;
}

std::bitset<11>
extract_11_bits (std::vector<bool>& packet)
{
  std::bitset<11> binary {};

  for (int i { 0 }; i < 11; ++i)
    {
      binary <<= 1;
      binary.set(0, packet.back());
      packet.pop_back();
    }

  return binary;
}

//------------------------------------------------
// Extracts a literal value from the given packet
//------------------------------------------------
std::bitset<64>
extract_literal (std::vector<bool>& packet)
{
  std::bitset<64> literal {};
  bool more_groups { true };
  int groups {};

  while (more_groups)
    {
      // check whether there are more groups after this
      more_groups = packet.back();
      packet.pop_back();

      // extract the group and place it into our literal
      for (int i { 0 }; i < 4; ++i)
        {
          literal <<= 1;
          literal.set(0, packet.back());
          packet.pop_back();
        }
      ++groups;
    }

  return literal;
}

//-------------------------------------------------------------------
// Calculates the result given a vector of values and the type ID of
// the operation to perform.
//-------------------------------------------------------------------
unsigned long
calc_result (const std::vector<unsigned long> values, unsigned long type_id)
{
  unsigned long result {};
  
  switch (type_id)
    {
    case 0:
      // sum of the values
      result = 0;
      for (unsigned long value : values)
        result += value;
      break;
    case 1:
      // product of the values
      result = 1;
      for (unsigned long value : values)
        result *= value;
      break;
    case 2:
      // minimum
      result = std::numeric_limits<unsigned long>::max();
      for (unsigned long value : values)
        result = (value < result) ? value : result;
      break;
    case 3:
      // maximum
      result = std::numeric_limits<unsigned long>::min();
      for (unsigned long value : values)
        result = (value > result) ? value : result;
      break;
    case 5:
      // greater than
      result = values[0] > values[1];
      break;
    case 6:
      // less than
      result = values[0] < values[1];
      break;
    case 7:
      // equalt to
      result = values[0] == values[1];
    }

  return result;
}

//----------------------------------------------------------------
// Decodes and evaluates the given packet.  Decoding is done in a
// destructive manner altering the given vector.
//----------------------------------------------------------------
unsigned long
decode_packet (std::vector<bool>& packet)
{
  unsigned long version_sum { extract_3_bits(packet).to_ulong() };
  unsigned long type_id { extract_3_bits(packet).to_ulong() };
  unsigned long result {};

  if (type_id == 4)
    result = extract_literal(packet).to_ulong();
  else
    {
      std::vector<unsigned long> subpacket_results {};
      bool length_id { packet.back() };
      packet.pop_back();

      // handle both length id cases
      if (length_id)
        {
          unsigned long num_subpackets { extract_11_bits(packet).to_ulong() };

          for (unsigned long i { 0 }; i < num_subpackets; ++i)
            subpacket_results.push_back(decode_packet(packet));      
        }
      else
        {
          unsigned long subpacket_bits { extract_15_bits(packet).to_ulong() };
          unsigned long target_size { packet.size() - subpacket_bits };

          while (packet.size() > target_size)
            subpacket_results.push_back(decode_packet(packet));
        }
      
      result = calc_result(subpacket_results, type_id);
    }
  
  return result;
}

int
main ()
{
  std::vector<bool> packet { read_input() };

  std::cout << decode_packet(packet) << '\n';
  
  return 0;
}
