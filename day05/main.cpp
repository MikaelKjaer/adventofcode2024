#include <utility>

#include "utility/utility.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  using order_t = std::pair<std::size_t, std::size_t>;
  using order_vector_t = std::vector<order_t>;
  using update_vector_t = std::vector<std::size_t>;
  using updates_vector_t = std::vector<update_vector_t>;
  using truth_table_t = std::vector<bool>;

  order_vector_t get_page_orders(const std::vector<std::string>& page_ordering)
  {
    order_vector_t orders;
    for (const auto& line : page_ordering)
    {
      order_t order;
      bool is_first = true;
      for (const auto& word : std::views::split(line, "|"sv))
      {
        std::size_t value = 0;
        if (auto [ptr, ec] = std::from_chars(word.data(), word.data() + word.size(), value); ec == std::errc{})
        {
          if (is_first)
          {
            order.first = value;
            is_first = false;
          }
          else
          {
            order.second = value;
          }
        }
      }
      orders.push_back(order);
    }
    return orders;
  }

  updates_vector_t get_updates(const std::vector<std::string>& page_update, std::size_t lower_bound)
  {
    updates_vector_t updates;
    for (const auto& line : page_update)
    {
      update_vector_t update;
      for (const auto& word : std::views::split(line, ","sv))
      {
        std::size_t value = 0;
        if (auto [ptr, ec] = std::from_chars(word.data(), word.data() + word.size(), value); ec == std::errc{})
        {
          auto calculated_value = value - lower_bound;
          update.push_back(calculated_value);
        }
      }
      updates.push_back(update);
    }
    return updates;
  }

  std::pair<std::size_t, std::size_t> find_bounds(const order_vector_t& orders)
  {
    std::size_t lower_bound = std::numeric_limits<std::size_t>::max();
    std::size_t upper_bound = std::numeric_limits<std::size_t>::min();

    for (const auto& order : orders)
    {
      lower_bound = std::min(order.first, lower_bound);
      lower_bound = std::min(order.second, lower_bound);

      upper_bound = std::max(order.first, upper_bound);
      upper_bound = std::max(order.second, upper_bound);
    }
    return { lower_bound, upper_bound };
  }

  auto get_order(const truth_table_t& range, std::size_t size, std::size_t x, std::size_t y) -> decltype(auto)
  {
    return range[x * size + y];
  }

  constexpr void set_order(truth_table_t& range, std::size_t size, std::size_t x, std::size_t y)
  {
    range[x * size + y] = true;
  }

  bool check_update(const truth_table_t& truth_table, std::size_t size, const update_vector_t& update)
  {
    for (auto it = std::begin(update); it != std::end(update); ++it)
    {
      for (auto it2 = std::next(it); it2 != std::end(update); ++it2)
      {
        if (get_order(truth_table, size, *it, *it2))
        {
          return false;
        }
      }
    }
    return true;
  }

  truth_table_t create_truth_table(const order_vector_t& orders, std::size_t size, std::size_t lower_bound)
  {
    truth_table_t truth_table;
    truth_table.resize(size * size);
    std::ranges::fill(truth_table, false);
    for (const auto& order : orders)
    {
      set_order(truth_table, size, order.second - lower_bound, order.first - lower_bound);
    }
    return truth_table;
  }

  std::size_t part1(std::size_t size, std::size_t lower_bound, truth_table_t& truth_table, const updates_vector_t& updates)
  {
    auto get_midpoint = [&lower_bound](auto&& range)
    {
      return lower_bound + range[range.size() / 2];
    };
    return std::ranges::fold_left(
      std::views::transform(
          std::views::filter(updates, std::bind_front(check_update, truth_table, size)), get_midpoint
        ),
        0,
      std::plus{});
  }

  std::size_t part2(std::size_t size, std::size_t lower_bound, truth_table_t& truth_table, updates_vector_t& updates)
  {
    auto get_midpoint = [&lower_bound](auto&& range)
    {
      return lower_bound + range[range.size() / 2];
    };
    auto sort_and_get_midpoint = [&size, &get_midpoint](const truth_table_t& inner_truth_table, update_vector_t& update)
    {
      bool did_swap = false;
      for (auto it = std::begin(update); it != std::end(update); ++it)
      {
        for (auto it2 = std::next(it); it2 != std::end(update); ++it2)
        {
          if (get_order(inner_truth_table, size, *it, *it2))
          {
            std::iter_swap(it, it2);
            did_swap = true;
          }
        }
      }
      return did_swap ? get_midpoint(update) : 0;
    };
    return std::ranges::fold_left(
      std::views::transform(updates, std::bind_front(sort_and_get_midpoint, truth_table)),
      0,
      std::plus{});
  }

}

int main(int, char**)
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  try
  {
    const input_t page_ordering = utility::read_file(std::filesystem::current_path() / "day05_1.txt"sv);
    const input_t page_update = utility::read_file(std::filesystem::current_path() / "day05_2.txt"sv);

    auto orders = get_page_orders(page_ordering);
    auto [lower_bound, upper_bound] = find_bounds(orders);
    auto size = upper_bound - lower_bound + 1;
    auto truth_table = create_truth_table(orders, size, lower_bound);
    auto updates = get_updates(page_update, lower_bound);

    fmt::print(FMT_STRING("Part 1: {0}\n"), part1(size, lower_bound, truth_table, updates));
    fmt::print(FMT_STRING("Part 2: {0}\n"), part2(size, lower_bound, truth_table, updates));
  }
  catch(...)
  {
    return -1;
  }
  return 0;
}
