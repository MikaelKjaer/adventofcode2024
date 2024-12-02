#include <execution>
#include <list>

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

  direction_t check_direction(std::int32_t value1, std::int32_t value2)
  {
    if (value1 < value2)
    {
      return direction_t::increasing;
    }
    if (value1 > value2)
    {
      return direction_t::decreasing;
    }
    return direction_t::unmoving;
  }

  bool report_is_safe(const int_vector_t& report)
  {
    auto direction = check_direction(report[0], report[1]);
    bool is_safe = true;
    for (std::size_t i = 0; i + 1 < report.size(); ++i)
    {
      auto difference = std::abs(report[i] - report[i + 1]);
      if (check_direction(report[i], report[i + 1]) == direction and
          direction != direction_t::unmoving and
          difference >= 1 and
          difference <= 3)
      {
        is_safe = true;
      }
      else
      {
        is_safe = false;
        break;
      }
    }
    return is_safe;
  }

  std::size_t part1(const std::vector<std::string>& input)
  {
    std::size_t safe = 0;
    for (const auto& line : input)
    {
      int_vector_t report;
      scn::ranges::subrange range(std::cbegin(line), std::cend(line));
      while (auto result = scn::scan<std::int32_t>(range, "{}"sv))
      {
        report.push_back(result->value());
        range = result->range();
      }

      if (not report.empty() and report.size() > 1)
      {
        if (report_is_safe(report))
        {
          ++safe;
        }
      }
    }

    return safe;
  }

  std::size_t part2(const std::vector<std::string>& input)
  {
    std::size_t safe = 0;
    for (const auto& line : input)
    {
      int_vector_t report;
      scn::ranges::subrange range(std::cbegin(line), std::cend(line));
      while (auto result = scn::scan<std::int32_t>(range, "{}"sv))
      {
        report.push_back(result->value());
        range = result->range();
      }

      if (not report.empty() and report.size() > 1)
      {
        for (std::size_t i = 0; i < report.size(); ++i)
        {
          auto copy = report;
          copy.erase(copy.begin() + static_cast<std::ptrdiff_t>(i));

          if (report_is_safe(copy))
          {
            ++safe;
            break;
          }
        }
      }
      else
      {
        fmt::println("No reports found\n"sv);
      }
    }

    return safe;
  }

}

int main(int, char**)
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  try
  {
    const std::vector<std::string> input = utility::read_file(std::filesystem::current_path() / "day02.txt"sv);
    fmt::print(FMT_STRING("Part 1: {0}\n"), part1(input));
    fmt::print(FMT_STRING("Part 2: {0}\n"), part2(input));
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}