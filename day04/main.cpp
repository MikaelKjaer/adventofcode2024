#include "utility/utility.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  bool is_xmas_string(char v1, char v2, char v3, char v4)
  {
    return (v1 == 'X' and v2 == 'M' and v3 == 'A' and v4 == 'S') or (v1 == 'S' and v2 =='A' and v3 =='M' and v4 =='X');
  }

  bool is_x_mas_shape(char top_left, char top_right, char center, char bottom_left, char bottom_right)
  {
    return ((top_left == 'M' and center =='A' and bottom_right == 'S') or
            (top_left == 'S' and center =='A' and bottom_right == 'M')) and
           ((top_right== 'M' and center =='A' and bottom_left == 'S') or
            (top_right== 'S' and center =='A' and bottom_left == 'M'));
  }

  char get(const std::string& line, std::size_t lines, std::size_t x, std::size_t y)
  {
    return line[y * lines + x];
  }

  std::size_t part1(const std::vector<std::string>& input)
  {
    std::size_t matches = 0;

    std::string line;
    for (const auto& l : input)
    {
      line.append(l);
    }

    constexpr static std::size_t pattern_width = 4;
    for(std::size_t y = 0; y < input.size(); ++y)
    {
      for(std::size_t x = 0; x <=(input[0].size() - pattern_width); ++x)
      {
        matches += is_xmas_string(get(line, input.size(), x, y), get(line, input.size(), x+1, y), get(line, input.size(), x+2, y), get(line, input.size(), x+3, y));
      }
    }

    for(std::size_t x=0; x < input[0].size(); ++x)
    {
      for(std::size_t y=0; y<=(input.size() - pattern_width); ++y)
      {
        matches += is_xmas_string(get(line, input.size(), x, y), get(line, input.size(), x,y+1), get(line, input.size(), x, y+2), get(line, input.size(), x, y+3));
      }
    }

    // search diagonally
    for(std::size_t x=3; x<input[0].size(); ++x)
    {
      for(std::size_t y=0; y<=(input.size() - pattern_width); ++y)
      {
        matches += is_xmas_string(get(line, input.size(), x, y), get(line, input.size(), x-1, y+1), get(line, input.size(), x-2, y+2), get(line, input.size(), x-3, y+3));
      }
    }
    for(std::size_t x=0; x<=(input[0].size() - pattern_width); ++x)
    {
      for(std::size_t y=0; y<=(input.size() - pattern_width); ++y)
      {
        matches += is_xmas_string(get(line, input.size(), x, y), get(line, input.size(), x+1, y+1), get(line, input.size(), x+2, y+2), get(line, input.size(), x+3, y+3));
      }
    }

    return matches;
  }

  std::size_t part2(const std::vector<std::string>& input)
  {
    std::size_t matches = 0;
    std::string line;
    for (const auto& l : input)
    {
      line.append(l);
    }

    for(std::size_t x=1; x < input[0].size() - 1; ++x)
    {
      for(int y=1; y < input.size() - 1; ++y)
      {
        matches += is_x_mas_shape(get(line, input.size(), x-1, y-1), get(line, input.size(), x+1, y-1), get(line, input.size(), x, y), get(line, input.size(), x-1, y+1), get(line, input.size(), x+1, y+1));
      }
    }
    return matches;
  }

}

int main(std::size_t, char**)
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  try
  {
    const std::vector<std::string> input = utility::read_file(std::filesystem::current_path() / "day04.txt"sv);
    fmt::print(FMT_STRING("Part 1: {0}\n"), part1(input));
    fmt::print(FMT_STRING("Part 2: {0}\n"), part2(input));
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}