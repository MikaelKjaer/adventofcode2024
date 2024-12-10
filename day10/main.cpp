#include <list>

#include "utility/utility.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{

  parse_result_t parse(const std::string& line)
  {
    return std::make_tuple(entries, last_file_index, first_free_space_index.value_or(0));
  }

  std::size_t part1(parse_result_t& result)
  {
    return 0;
  }

  std::size_t part2(parse_result_t& result)
  {
    return 0;
  }

}

int main(int, char**)
{
  try
  {
    const input_t input = utility::read_file(std::filesystem::current_path() / "day10.txt"sv);
    auto parsed = parse(input[0]);
    auto parsed_part2 = parsed;
    utility::run_part(1, [](auto& p){ return part1(p); }, parsed);
    utility::run_part(2, [](auto& p){ return part2(p); }, parsed_part2);
  }
  catch (const std::exception& e)
  {
    fmt::print("Caught exception in main: {}\n", e.what());
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}
