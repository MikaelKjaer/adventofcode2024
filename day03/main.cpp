#include "utility/utility.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  using int_vector_t = std::vector<std::int32_t>;

  enum class direction_t : std::uint8_t
  {
    increasing,
    unmoving,
    decreasing
  };

  std::size_t part1(const std::vector<std::string>& input)
  {
    std::size_t sum = 0;
    for (const auto& line : input)
    {
      scn::ranges::subrange range(std::cbegin(line), std::cend(line));
      while (auto result = scn::scan<std::int32_t, int32_t>(range, "mul({},{})"sv))
      {
        auto [v1, v2] = result->values();
        sum += v1 * v2;
        range = result->range();
      }
    }

    return sum;
  }

}

int main(int, char**)
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  try
  {
    const std::vector<std::string> input = utility::read_file(std::filesystem::current_path() / "day03.txt"sv);
    fmt::print(FMT_STRING("Part 1: {0}\n"), part1(input));
    //fmt::print(FMT_STRING("Part 2: {0}\n"), part2(input));
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}