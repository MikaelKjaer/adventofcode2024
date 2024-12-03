#include "utility/utility.hpp"

namespace
{
  using int_vector_t = std::vector<int>;
  using int_vector_tuple_t = std::tuple<int_vector_t, int_vector_t>;

  int_vector_tuple_t get_data(const std::vector<std::string>& input)
  {
    std::vector<std::int32_t> first;
    std::vector<std::int32_t> second;

    for (const auto& string : input)
    {
      using namespace std::string_literals;
      using namespace std::string_view_literals;

      bool is_first = true;
      for (const auto& word : std::views::split(string, "   "s))
      {
        std::int32_t value = 0;
        if (auto [ptr, ec] = std::from_chars(word.data(), word.data() + word.size(), value); ec == std::errc{})
        {
          if (is_first)
          {
            first.push_back(value);
          }
          else
          {
            second.push_back(value);
          }
        }
        is_first = !is_first;
      }
    }
    std::ranges::sort(first);
    std::ranges::sort(second);

    return { first, second };
  }

  std::size_t part1(const std::vector<std::string>& input)
  {
    std::size_t distance = 0;

    auto [first, second] = get_data(input);

    auto zip_view = std::views::zip(first, second);

    for (const auto& [v1, v2] : zip_view)
    {
      distance += static_cast<std::size_t>(std::abs(v1 - v2));
    }
    return distance;
  }

  std::size_t part2(const std::vector<std::string>& input)
  {
    auto [first, second] = get_data(input);

    std::size_t similarity = 0;
    for (const auto& v1 : first)
    {
      auto range = std::equal_range(second.begin(), second.end(), v1);
      similarity += static_cast<std::size_t>(v1 * std::distance(range.first, range.second));
    }

    return similarity;
  }
}

int main(int, char**)
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  try
  {
    const std::vector<std::string> input = utility::read_file(std::filesystem::current_path() / "day01.txt"sv);
    fmt::print(FMT_STRING("Part 1 distance: {0}\n"), part1(input));
    fmt::print(FMT_STRING("Part 2 similarity: {0}\n"), part2(input));
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}