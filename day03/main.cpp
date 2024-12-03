#include "utility/utility.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  enum class state_t
  {
    not_found,
    m_state,
    u_state,
    l_state,
    open_parens_state,
    digit_state_1,
    comma_state,
    digit_state_2,
  };

  enum class state2_t
  {
    not_found,
    m_state,
    u_state,
    l_state,
    open_parens_state,
    digit_state_1,
    comma_state,
    digit_state_2,
    d_state,
    o_state,
    do_open_parens_state,
    n_state,
    accent_state,
    t_state,
    do_not_open_parens_state
  };

  using result_t = std::optional<std::pair<std::int32_t, std::int32_t>>;

  result_t find_next_match(std::string::const_iterator& it, const std::string::const_iterator& end)
  {
    state_t state = state_t::not_found;
    std::stringstream ss_v1, ss_v2;
    for (; it != end; ++it)
    {
      if (*it == 'm')
      {
        state = state_t::m_state;
      }
      else if (state == state_t::m_state and *it == 'u')
      {
        state = state_t::u_state;
      }
      else if (state == state_t::u_state and *it == 'l')
      {
        state = state_t::l_state;
      }
      else if (state == state_t::l_state and *it == '(')
      {
        state = state_t::open_parens_state;
      }
      else if (state == state_t::open_parens_state and std::isdigit(*it))
      {
        state = state_t::digit_state_1;
        ss_v1 << *it;
      }
      else if (state == state_t::digit_state_1 and std::isdigit(*it))
      {
        ss_v1 << *it;
      }
      else if (state == state_t::digit_state_1 and *it == ',')
      {
        state = state_t::comma_state;
      }
      else if (state == state_t::comma_state and std::isdigit(*it))
      {
        state = state_t::digit_state_2;
        ss_v2 << *it;
      }
      else if (state == state_t::digit_state_2 and std::isdigit(*it))
      {
        ss_v2 << *it;
      }
      else if (state == state_t::digit_state_2 and *it == ')')
      {
        auto string1 = ss_v1.str();
        std::int32_t v1 = 0;
        auto result1 = std::from_chars(string1.data(), string1.data() + string1.size(), v1);

        auto string2 = ss_v2.str();
        std::int32_t v2 = 0;
        auto result2 = std::from_chars(string2.data(), string2.data() + string2.size(), v2);

        if (result1.ec == std::errc() and result2.ec == std::errc())
        {
          return std::pair{ v1, v2 };
        }
        ss_v1.str("");
        ss_v2.str("");
      }
      else
      {
        ss_v1.str("");
        ss_v2.str("");
        state = state_t::not_found;
      }
    }

    return {};
  }

  result_t find_next_match_part2(std::string::const_iterator& it, const std::string::const_iterator& end, bool& active)
  {
    state2_t state = state2_t::not_found;
    std::stringstream ss_v1, ss_v2;

    for (; it != end; ++it)
    {
      if (active)
      {
        if (state == state2_t::not_found and *it == 'd')
        {
          state = state2_t::d_state;
        }
        else if (state == state2_t::d_state and *it == 'o')
        {
          state = state2_t::o_state;
        }
        else if (state == state2_t::o_state and *it == 'n')
        {
          state = state2_t::n_state;
        }
        else if (state == state2_t::n_state and *it == '\'')
        {
          state = state2_t::accent_state;
        }
        else if (state == state2_t::accent_state and *it == 't')
        {
          state = state2_t::t_state;
        }
        else if (state == state2_t::t_state and *it == '(')
        {
          state = state2_t::do_not_open_parens_state;
        }
        else if (state == state2_t::do_not_open_parens_state and *it == ')')
        {
          active = false;
          state = state2_t::not_found;
        }
        else if (state == state2_t::not_found and *it == 'm')
        {
          state = state2_t::m_state;
        }
        else if (state == state2_t::m_state and *it == 'u')
        {
          state = state2_t::u_state;
        }
        else if (state == state2_t::u_state and *it == 'l')
        {
          state = state2_t::l_state;
        }
        else if (state == state2_t::l_state and *it == '(')
        {
          state = state2_t::open_parens_state;
        }
        else if (state == state2_t::open_parens_state and std::isdigit(*it))
        {
          state = state2_t::digit_state_1;
          ss_v1 << *it;
        }
        else if (state == state2_t::digit_state_1 and std::isdigit(*it))
        {
          ss_v1 << *it;
        }
        else if (state == state2_t::digit_state_1 and *it == ',')
        {
          state = state2_t::comma_state;
        }
        else if (state == state2_t::comma_state and std::isdigit(*it))
        {
          state = state2_t::digit_state_2;
          ss_v2 << *it;
        }
        else if (state == state2_t::digit_state_2 and std::isdigit(*it))
        {
          ss_v2 << *it;
        }
        else if (state == state2_t::digit_state_2 and *it == ')')
        {
          auto string1 = ss_v1.str();
          std::int32_t v1 = 0;
          auto result1 = std::from_chars(string1.data(), string1.data() + string1.size(), v1);

          auto string2 = ss_v2.str();
          std::int32_t v2 = 0;
          auto result2 = std::from_chars(string2.data(), string2.data() + string2.size(), v2);

          if (result1.ec == std::errc() and result2.ec == std::errc())
          {
            return std::pair{ v1, v2 };
          }
          ss_v1.str("");
          ss_v2.str("");
        }
        else
        {
          ss_v1.str("");
          ss_v2.str("");
          state = state2_t::not_found;
        }
      }
      else if (state == state2_t::not_found and *it == 'd')
      {
        state = state2_t::d_state;
      }
      else if (state == state2_t::d_state and *it == 'o')
      {
        state = state2_t::o_state;
      }
      else if (state == state2_t::o_state and *it == '(')
      {
        state = state2_t::do_open_parens_state;
      }
      else if (state == state2_t::do_open_parens_state and *it == ')')
      {
        state = state2_t::not_found;
        active = true;
      }
      else
      {
        ss_v1.str("");
        ss_v2.str("");
        state = state2_t::not_found;
      }
    }

    return {};
  }

  std::size_t part1(const std::vector<std::string>& input)
  {
    std::size_t sum = 0;
    for (const auto& line : input)
    {
      auto it = line.begin();
      while (auto result = find_next_match(it, line.end()))
      {
        if (result)
        {
          auto value = *result;
          sum += (value.first * value.second);
        }
        if (it == line.end())
        {
          break;
        }
      }
    }
    return sum;
  }

  std::size_t part2(const std::vector<std::string>& input)
  {
    std::size_t sum = 0;
    bool active = true;
    for (const auto& line : input)
    {
      auto it = line.begin();
      while (auto result = find_next_match_part2(it, line.end(), active))
      {
        if (result)
        {
          auto value = *result;
          sum += (value.first * value.second);
        }
        if (it == line.end())
        {
          break;
        }
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
    fmt::print(FMT_STRING("Part 2: {0}\n"), part2(input));
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}