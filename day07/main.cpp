#include "utility/utility.hpp"
#include <scn/scan.h>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  using factor_vector_t = std::vector<std::uint64_t>;

  struct entry_t
  {
    constexpr entry_t() :
      end_value{0}
    {}
    std::uint64_t end_value;
    factor_vector_t factors;
  };

  using entry_vector_t = std::vector<entry_t>;

  struct concatenate_t
  {
    constexpr auto operator()(std::integral auto value1, std::integral auto value2) const -> decltype(value1 + value2)
    {
      return value1 * std::pow(10, static_cast<decltype(value1)>(std::log10(value2)) + 1) + value2;
    }
  };

  enum class e_operator_t : std::uint8_t
  {
    none = 0,
    plus = 1,
    multiplies = 2,
    concatenate = 3
  };

  using operator_t = std::pair<e_operator_t, std::function<std::uint64_t(std::uint64_t, std::uint64_t)>>;

  entry_vector_t parse(const input_t& input)
  {
    entry_vector_t entries;

    for (const auto& line : input)
    {
      entry_t entry;
      for (const auto& word : std::views::split(line, ": "sv))
      {
        if (entry.end_value == 0)
        {
          auto [ptr, ec] = std::from_chars(word.data(), word.data() + word.size(), entry.end_value);
          if (ec != std::errc{})
          {
            break;
          }
        }
        else
        {
          scn::ranges::subrange range(std::cbegin(word), std::cend(word));
          while (auto result = scn::scan<std::uint64_t>(range, "{}"sv))
          {
            entry.factors.push_back(result->value());
            range = result->range();
          }
        }
      }
      if (entry.end_value != 0 and not entry.factors.empty())
      {
        entries.push_back(entry);
      }
    }
    return entries;
  }

  bool can_be_made_true(const entry_t& entry, std::uint64_t previous, std::size_t next, auto operation)
  {
    auto new_result = operation(previous, entry.factors[next]);
    if (new_result == entry.end_value)
    {
      return true;
    }
    if (next + 1 >= entry.factors.size() or new_result > entry.end_value)
    {
      return false;
    }
    return can_be_made_true(entry, new_result, next + 1, std::multiplies{}) or can_be_made_true(entry, new_result, next + 1, std::plus{});
  }

  std::size_t part1(const entry_vector_t& entries)
  {
    std::size_t result = 0;
    for (const auto& entry : entries)
    {
      if (entry.factors.size() >= 2 and (can_be_made_true(entry, entry.factors[0], 1,  std::multiplies{}) or can_be_made_true(entry, entry.factors[0], 1, std::plus{})))
      {
        result += static_cast<std::size_t>(entry.end_value);
      }
    }
    return result;
  }

  constexpr bool solve_entry(std::uint64_t end_result, std::span<const std::uint64_t> factors, std::optional<std::uint64_t> accumulated, std::span<const operator_t> operators)
  {
    if (factors.size() == 1)
    {
      for (const auto& op : operators)
      {
        const auto result = op.second(*accumulated, factors.front());
        if (result == end_result)
        {
          return true;
        }
      }
      return false;
    }

    for (const auto& operand : operators)
    {
      const auto result = !accumulated ? factors.front() : operand.second(*accumulated, factors.front());
      if (result > end_result)
      {
        continue;
      }
      auto found = solve_entry(end_result, { std::next(factors.begin()), factors.end() }, result, operators);
      if (found)
      {
        return true;
      }
    }

    return false;
  }

  std::size_t part2(const entry_vector_t& entries)
  {
    static const std::array<operator_t, 3> operations =
    {
      std::make_pair(e_operator_t::plus, std::plus{}),
      std::make_pair(e_operator_t::multiplies, std::multiplies{}),
      std::make_pair(e_operator_t::concatenate, concatenate_t{})
    };
    auto tmp = entries | std::views::transform([](const entry_t& entry)
    {
      return std::make_pair(entry.end_value, solve_entry(entry.end_value, entry.factors, std::nullopt, operations));
    });

    auto intermediate_results = std::vector<std::pair<std::uint64_t, bool>>{ tmp.begin(), tmp.end() };

    auto valid_results = intermediate_results | std::views::filter([](const std::pair<std::uint64_t, bool>& result) { return result.second; }) | std::views::transform([](const auto& p){ return p.first; });

    return std::reduce(valid_results.begin(), valid_results.end(), 0ull);
  }

}

int main(int, char**)
{
  try
  {
    const input_t input = utility::read_file(std::filesystem::current_path() / "day07.txt"sv);
    auto parsed = parse(input);
    utility::run_part(1, [](const entry_vector_t& entries){ return part1(entries); }, parsed);
    utility::run_part(2, [](const entry_vector_t& entries){ return part2(entries); }, parsed);
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}