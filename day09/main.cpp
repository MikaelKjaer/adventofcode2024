#include "utility/utility.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  std::size_t part1(const std::string& line)
  {
    return 0;
  }

  std::size_t part2(const std::string& line)
  {
    return 0;
  }

}

int main(int, char**)
{
  try
  {
    const input_t input = utility::read_file(std::filesystem::current_path() / "day08.txt"sv);
    utility::run_part(1, [](const auto& parsed){ return part1(parsed); }, input[0]);
    utility::run_part(2, [](const auto& parsed){ return part2(parsed); }, input[0]);
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}