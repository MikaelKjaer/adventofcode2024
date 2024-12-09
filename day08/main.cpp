#include "utility/utility.hpp"
#include <scn/scan.h>

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  struct position
  {
    constexpr position() = default;
    constexpr position(std::int32_t px, std::int32_t py) :
      x(px), y(py)
    {}
    constexpr position(const position&) = default;
    constexpr position(position&&) noexcept = default;
    constexpr ~position() noexcept = default;
    constexpr position& operator=(const position&) = default;
    constexpr position& operator=(position&&) noexcept = default;
    constexpr auto operator<=>(const position&) const = default;

    constexpr position operator-(const position& other) const noexcept
    {
      return position{ x - other.x, y - other.y };
    }

    constexpr position operator+(const position& other) const noexcept
    {
      return position{ x + other.x, y + other.y };
    }

    std::int32_t x;
    std::int32_t y;
  };

  using locations_t = std::map<std::int8_t, std::set<position>>;

  struct map_t
  {
    constexpr map_t() :
      width{0}, height{0}
    {}
    map_t(const map_t&) = default;
    map_t(map_t&&) noexcept = default;
    ~map_t() = default;
    map_t& operator=(const map_t&) = default;
    map_t& operator=(map_t&&) noexcept = default;

    locations_t antenna_locations;
    std::int32_t width;
    std::int32_t height;
  };

  map_t parse(const input_t& input)
  {
    map_t result;
    for (std::int32_t y = 0; y < input.size(); ++y)
    {
      auto& line = input[y];
      for (std::int32_t x = 0; x < line.size(); ++x)
      {
        const auto character = line[x];
        if ((character >= 'A' and character <= 'Z') or (character >= 'a' and character <= 'z') or (character >= '0' and character <= '9'))
        {
          result.antenna_locations[character].insert(position{ x, y });
        }
      }
    }
    result.height = static_cast<std::int32_t>(input.size());
    result.width = static_cast<std::int32_t>(input[0].size());
    return result;
  }

  constexpr bool is_within_bounds(const position& pos, std::int32_t width, std::int32_t height)
  {
    return pos.x >= 0 and pos.x < width and pos.y >= 0 and pos.y < height;
  }

  std::size_t part1(const map_t& data)
  {
    std::set<position> anti_nodes{};
    for (const auto& [antenna, locations] : data.antenna_locations)
    {
      for (auto iter_from = std::begin(locations); iter_from != std::end(locations); ++iter_from)
      {
        for (auto iter = std::next(iter_from); iter != locations.end(); ++iter)
        {
          const auto distance = *iter - *iter_from;
          const auto loc1 = *iter + distance;
          const auto loc2 = *iter_from - distance;
          if (is_within_bounds(loc1, data.width, data.height))
          {
            anti_nodes.insert(loc1);
          }
          if (is_within_bounds(loc2, data.width, data.height))
          {
            anti_nodes.insert(loc2);
          }
        }
      }
    }
    return anti_nodes.size();
  }

  std::size_t part2(const map_t& data)
  {
    std::set<position> anti_nodes{};
    for (const auto& [antenna, locations] : data.antenna_locations)
    {
      for (auto iter_from = std::begin(locations); iter_from != std::end(locations); ++iter_from)
      {
        for (auto iter = std::next(iter_from); iter != locations.end(); ++iter)
        {
          const auto distance = *iter - *iter_from;
          auto location = *iter_from;
          while (is_within_bounds(location, data.width, data.height))
          {
            anti_nodes.insert(location);
            location = location + distance;
          }
          location = *iter_from - distance;
          while (is_within_bounds(location, data.width, data.height))
          {
            anti_nodes.insert(location);
            location = location - distance;
          }
        }
      }
    }
    return anti_nodes.size();
  }

}

int main(int, char**)
{
  try
  {
    const input_t input = utility::read_file(std::filesystem::current_path() / "day08.txt"sv);
    auto parsed = parse(input);
    utility::run_part(1, [](const auto& parsed){ return part1(parsed); }, parsed);
    utility::run_part(2, [](const auto& parsed){ return part2(parsed); }, parsed);
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}