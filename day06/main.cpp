#include <pstl/glue_execution_defs.h>

#include "utility/utility.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  enum class move_result_t : std::uint8_t
  {
    blocked,
    unblocked,
    out_of_bounds,
    looping
  };

  using position_t = std::pair<std::int32_t, std::int32_t>;
  using position_map_t = std::map<position_t, position_t>;

  struct guard_t
  {
    explicit guard_t(const position_t& pos) :
      total_moves(1), x(pos.first), y(pos.second), current_direction{0l}, looping{false}
    {
      all_moves[pos] = directions[current_direction];
    }

    guard_t(const guard_t&) = default;
    guard_t(guard_t&&) noexcept = default;
    ~guard_t() = default;

    guard_t& operator=(const guard_t&) = default;
    guard_t& operator=(guard_t&&) noexcept = default;

    [[nodiscard]] move_result_t can_move(const input_t& input) const
    {
      if (looping)
      {
        return move_result_t::looping;
      }
      auto direction = directions[current_direction];
      if (x + direction.first < 0 or x + direction.first >= input.size())
      {
        return move_result_t::out_of_bounds;
      }
      if (y + direction.second < 0 or y + direction.second >= input[x].size())
      {
        return move_result_t::out_of_bounds;
      }

      if (input[x + direction.first][y + direction.second] == '#')
      {
        return move_result_t::blocked;
      }
      return move_result_t::unblocked;
    }

    void turn_right()
    {
      if (current_direction + 1 < directions.size())
      {
        ++current_direction;
      }
      else
      {
        current_direction = 0;
      }
    }

    void move()
    {
      auto direction = directions[current_direction];
      x += direction.first;
      y += direction.second;

      position_t new_pos{x, y};
      auto found = all_moves.find(new_pos);
      if (found == all_moves.end())
      {
        ++total_moves;
        all_moves[new_pos] = direction;;
      }
      if (found != all_moves.end() and found->second == direction)
      {
        looping = true;
      }

    }

    [[nodiscard]] std::size_t moves() const
    {
      return total_moves;
    }

    [[nodiscard]] position_map_t all_positions() const
    {
      return all_moves;
    }
  private:
    constexpr static std::array<position_t, 4> directions{ position_t{ -1, 0}, position_t{ 0, 1 }, position_t{ 1, 0 }, position_t{ 0, -1 } };

    std::size_t total_moves;
    std::int32_t x;
    std::int32_t y;
    std::size_t current_direction;
    bool looping;
    position_map_t all_moves;
  };

  position_t find_guard(input_t& input)
  {
    position_t result = std::make_pair(0, 0);
    for (std::size_t i = 0; i < input.size(); ++i)
    {
      for (std::size_t j = 0; j < input[i].size(); ++j)
      {
        if (input[i][j] == '^')
        {
          input[i][j] = '.';
          result.first = static_cast<std::int32_t>(i);
          result.second = static_cast<std::int32_t>(j);
          break;
        }
      }
    }
    return result;
  }

  move_result_t run_through(const input_t& input, guard_t& guard)
  {
    move_result_t result;
    do
    {
      result = guard.can_move(input);
      if (result == move_result_t::blocked)
      {
        guard.turn_right();
      }
      else if (result == move_result_t::unblocked)
      {
        guard.move();
      }
    } while (result != move_result_t::out_of_bounds and result != move_result_t::looping);

    return result;
  }

  std::size_t part1(const input_t& input)
  {
    auto copied_input = input;
    auto guard_position = find_guard(copied_input);
    guard_t guard(guard_position);

    run_through(copied_input, guard);
    return guard.moves();
  }

  std::size_t part2(const input_t& input)
  {
    auto copied_input = input;
    std::size_t obstructed_paths = 0;
    auto guard_position = find_guard(copied_input);
    guard_t guard(guard_position);

    run_through(copied_input, guard);
    auto path = guard.all_positions();

    std::ranges::for_each(path, [&obstructed_paths, &copied_input, &guard_position](const auto& entry)
    {
      auto position = entry.first;
      copied_input[position.first][position.second] = '#';

      guard_t new_guard(guard_position);
      auto result = run_through(copied_input, new_guard);
      if (result == move_result_t::looping)
      {
        ++obstructed_paths;
      }
      copied_input[position.first][position.second] = '.';
    });
    return obstructed_paths;
  }

}

int main(int, char**)
{
  try
  {
    input_t input = utility::read_file(std::filesystem::current_path() / "day06.txt"sv);
    utility::run_part(1, [](const input_t& input){ return part1(input); }, input);
    utility::run_part(2, [](const input_t& input){ return part2(input); }, input);
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}