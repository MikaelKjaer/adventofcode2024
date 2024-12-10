#include "utility/utility.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  template<typename T>
  struct coordinate
  {
    constexpr bool operator<(const coordinate& p) const
    {
      if (x < p.x)
      {
        return true;
      }
      if (p.x < x)
      {
        return false;
      }
      return y < p.y;
    }

    constexpr coordinate operator + (const coordinate& p) const
    {
      return coordinate(x + p.x, y + p.y);
    }

    template<typename U> requires std::is_integral_v<T>
    [[nodiscard]] bool within(const U& map) const
    {
      return x >= 0 and y >= 0 and static_cast<std::size_t>(x) < map.size() and static_cast<std::size_t>(y) < map[static_cast<std::size_t>(x)].size();
    }

    template<typename U> requires std::is_integral_v<T>
    [[nodiscard]] const auto& get_at(const U& map) const
    {
      return map[static_cast<std::size_t>(x)][static_cast<std::size_t>(y)];
    }

    T x = {}, y = {};
  };

  using point_t = coordinate<std::int32_t>;
  constexpr std::array directions = { point_t(1, 0), point_t(0, 1), point_t(-1, 0), point_t(0,-1) };

  template <typename T>
  class y_combinator
  {
    T lambda;
  public:
    constexpr explicit y_combinator(T&& t) :
      lambda(std::forward<T>(t))
    {}

    template <typename...Args>
    constexpr decltype(auto) operator()(Args&&...args) const
    {
      return lambda(std::move(*this), std::forward<Args>(args)...);
    }
  };

  using map_line_t = std::vector<std::int32_t>;
  using topological_map_t = std::vector<map_line_t>;

  topological_map_t parse(const input_t& input)
  {
    topological_map_t map;
    for (const auto& line : input)
    {
      map_line_t map_line;
      for (auto character : line)
      {
        std::uint8_t height = 0;
        auto [ptr, ec] = std::from_chars(&character, &character + sizeof(char), height, 10);
        if (ec == std::errc{})
        {
          map_line.push_back(height);
        }
      }
      map_line.shrink_to_fit();
      map.push_back(map_line);
    }
    map.shrink_to_fit();
    return map;
  }

  std::pair<std::int32_t,std::int32_t> both_parts(const topological_map_t& map)
  {
    std::int32_t part1_sum = 0, part2_sum = 0;
    auto PathCount = y_combinator([&map](auto && functor, point_t p, std::int32_t step) -> std::int32_t
    {
      if (step++ >= 9)
      {
        return 1;
      }
      std::int32_t result = 0;
      for (point_t d : directions)
      {
        if (point_t np = p + d; np.within(map) and np.get_at(map) == step)
        {
          result += functor(np, step);
        }
      }

      return result;
    });

    for (std::int32_t i = 0; static_cast<std::size_t>(i) < map.size(); ++i)
    {
      for (std::int32_t j = 0; static_cast<std::size_t>(j) < map.front().size(); ++j)
      {
        if (map[static_cast<std::size_t>(i)][static_cast<std::size_t>(j)] == 0)
        {
          std::set<point_t> current, next;
          current.insert({i, j});
          std::int32_t iter = 1;
          while (iter < 10 and not current.empty())
          {
            for (point_t p : current)
            {
              for (point_t direction : directions)
              {
                if (point_t np = p + direction; np.within(map) && np.get_at(map) == iter)
                {
                  next.insert(np);
                }
              }
            }
            ++iter;
            std::swap(current, next);
            next.clear();
          }
          part1_sum += static_cast<std::int32_t>(current.size());
          part2_sum += PathCount(point_t{i, j}, 0);
        }
      }
    }
    return { part1_sum, part2_sum };
  }

}

int main(int, char**)
{
  try
  {
    const input_t input = utility::read_file(std::filesystem::current_path() / "day10.txt"sv);
    auto parsed = parse(input);
    auto start = std::chrono::high_resolution_clock::now();
    auto result = both_parts(parsed);
    auto end = std::chrono::high_resolution_clock::now();
    auto time_taken = end - start;
    fmt::print(FMT_STRING("Part 1: {}.\nPart 2: {}.\nBoth done in {}us\n"), result.first, result.second, std::chrono::duration_cast<std::chrono::microseconds>(time_taken).count());
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
