#include "utility/utility.hpp"
#include "scn/scan.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  using stone_t = std::uint64_t;
  using stones_t = std::list<stone_t>;

  struct cache_key
  {
    stone_t result;
    stone_t value;
    stone_t blinks;

    constexpr bool operator==(const cache_key& rhs) const noexcept
    {
      return value == rhs.value && blinks == rhs.blinks;
    }
  };
}

  template<>
  struct std::hash<cache_key>
  {
    auto operator()(const cache_key& key) const noexcept
    {
      return std::hash<decltype(key.value)>{}(key.value) ^ (key.blinks << (sizeof(decltype(key.value)) - sizeof(decltype(key.blinks))) * 8);
    }
  };

namespace
{
  using cache_t = std::unordered_set<cache_key>;
  constexpr std::uint64_t rule_3_factor = 2024;

  constexpr std::pair<bool, std::uint64_t> find_digits(stone_t& x)
  {
    if (x >= 10000000000)
    {
      if (x >= 100000000000000)
      {
        if (x >= 10000000000000000)
        {
          if (x >= 100000000000000000)
          {
            if (x >= 1000000000000000000)
            {
              return {false, static_cast<std::uint64_t>(std::pow(10, 19/2))};
            }
            return {true, static_cast<std::uint64_t>(std::pow(10, 18/2))};
          }
          return {false, static_cast<std::uint64_t>(std::pow(10, 17/2))};;
        }
        if (x >= 1000000000000000)
        {
          return {true, static_cast<std::uint64_t>(std::pow(10, 16/2))};;
        }
        return {false, static_cast<std::uint64_t>(std::pow(10, 15/2))};;
      }
      if (x >= 1000000000000)
      {
        if (x >= 10000000000000)
        {
          return {true, static_cast<std::uint64_t>(std::pow(10, 14/2))};
        }
        return {false, static_cast<std::uint64_t>(std::pow(10, 13/2))};
      }
      if (x >= 100000000000)
      {
        return {true, static_cast<std::uint64_t>(std::pow(10, 12/2))};;
      }
      return {false, static_cast<std::uint64_t>(std::pow(10, 11/2))};
    }
    if (x >= 100000)
    {
      if (x >= 10000000)
      {
        if (x >= 100000000)
        {
          if (x >= 1000000000)
          {
            return {true, static_cast<std::uint64_t>(std::pow(10, 10/2))};
          }
          return {false, static_cast<std::uint64_t>(std::pow(10, 9/2))};
        }
        return {true, static_cast<std::uint64_t>(std::pow(10, 8/2))};
      }
      if (x >= 1000000)
      {
        return {false, static_cast<std::uint64_t>(std::pow(10, 7/2))};
      }
      return {true, static_cast<std::uint64_t>(std::pow(10, 6/2))};
    }
    if (x >= 100)
    {
      if (x >= 1000)
      {
        if (x >= 10000)
        {
          return {false, static_cast<std::uint64_t>(std::pow(10, 5/2))};
        }
        return {true, static_cast<std::uint64_t>(std::pow(10, 4/2))};
      }
      return {false, static_cast<std::uint64_t>(std::pow(10, 3/2))};
    }
    if (x >= 10)
    {
      return {true, static_cast<std::uint64_t>(std::pow(10, 1))};;
    }
    return {false, static_cast<std::uint64_t>(std::pow(10, 1/2))};
  }

  stones_t parse(const std::string& line)
  {
    stones_t stones;
    scn::ranges::subrange range(std::cbegin(line), std::cend(line));
    while (auto result = scn::scan<std::uint64_t>(range, "{}"sv))
    {
      stones.push_back(result->value());
      range = result->range();
    }
    return stones;
  }

  template<size_t Blinks, bool UseCache = true> requires (Blinks > 0)
  constexpr stone_t blink(cache_t& cache, stone_t stone) noexcept
  {
    if (stone == 0)
    {
      return blink<Blinks - 1, false>(cache, 1);
    }

    if constexpr (UseCache)
    {
      if (auto value = cache.find({ 0, stone, Blinks }); value != cache.end())
      {
        return value->result;
      }
    }

    stone_t ret;
    if (auto digits = find_digits(stone); digits.first)
    {
      auto new_value_1 = stone / digits.second;
      auto new_value_2 = stone % digits.second;
      ret = blink<Blinks - 1>(cache, new_value_1) + blink<Blinks - 1>(cache, new_value_2);
    }
    else
    {
      ret = blink<Blinks - 1>(cache, stone * rule_3_factor);
    }
    if constexpr (UseCache)
    {
      cache.emplace(ret, stone, Blinks);
    }
    return ret;
  }

  template<size_t Blinks, bool UseCache = false> requires (Blinks == 0)
  constexpr stone_t blink(cache_t&, stone_t) noexcept
  {
    return 1;
  }

  template<std::size_t Blinks>
  std::size_t handle_part(stones_t& stones)
  {
    return std::ranges::fold_left(stones, stone_t{}, [](stone_t initial, stone_t stone)
    {
      cache_t cache;
      return initial + blink<Blinks>(cache, stone);
    });
  }
}

int main(int, char**)
{
  try
  {
    const input_t input = utility::read_file(std::filesystem::current_path() / "day11.txt"sv);
    auto parsed = parse(input[0]);
    utility::run_part(1, [](auto& p){ return handle_part<25>(p); }, parsed);
    utility::run_part(2, [](auto& p){ return handle_part<75>(p); }, parsed);
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
