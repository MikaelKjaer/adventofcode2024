#include <list>

#include "utility/utility.hpp"

using namespace std::string_literals;
using namespace std::string_view_literals;

namespace
{
  struct file_t
  {
    constexpr file_t () :
      file_number(0), length(0)
    {}
    constexpr explicit file_t (std::size_t n, std::uint8_t len) :
      file_number(n), length(len)
    {}
    constexpr file_t(const file_t&) = default;
    constexpr file_t(file_t&&) = default;
    constexpr ~file_t() = default;
    constexpr file_t& operator=(const file_t&) = default;
    constexpr file_t& operator=(file_t&&) = default;

    constexpr bool operator==(const file_t& rhs) const = default;
    constexpr auto operator<=>(const file_t&) const = default;

    std::size_t file_number;
    std::uint8_t length;
  };

  struct free_space_t
  {
    constexpr free_space_t () = default;
    constexpr free_space_t(const free_space_t&) = default;
    constexpr free_space_t(free_space_t&&) = default;
    constexpr ~free_space_t() = default;
    constexpr free_space_t& operator=(const free_space_t&) = default;
    constexpr free_space_t& operator=(free_space_t&&) = default;
    constexpr bool operator==(const free_space_t& rhs) const = default;
    constexpr auto operator<=>(const free_space_t&) const = default;

  };

  using entry_t = std::variant<free_space_t, file_t>;
  using entry_list_t = std::vector<entry_t>;
  using parse_result_t = std::tuple<entry_list_t, std::size_t, std::size_t>;

  parse_result_t parse(const std::string& line)
  {
    entry_list_t entries;

    std::size_t file_number = 0;
    std::size_t index = 0;
    std::size_t last_file_index = 0;
    std::optional<std::size_t> first_free_space_index;
    bool is_file = true;
    for (auto character : line)
    {
      std::uint8_t length = 0;
      auto [ptr, ec] = std::from_chars(&character, &character + sizeof(char), length, 10);
      if (ec == std::errc{})
      {
        if (is_file)
        {
          for (auto i = 0; i < length; ++i)
          {
            entries.emplace_back(file_t(file_number, length));
            ++index;
            last_file_index = index;
          }
          ++file_number;
        }
        else
        {
          for (auto i = 0; i < length; ++i)
          {
            entries.emplace_back(free_space_t());
            if (not first_free_space_index)
            {
              first_free_space_index = index;
            }
            ++index;
          }
        }
        is_file = !is_file;
      }
    }
    entries.shrink_to_fit();
    return std::make_tuple(entries, last_file_index, first_free_space_index.value_or(0));
  }

  std::size_t checksum(entry_list_t& entries)
  {
    std::size_t checksum = 0;
    std::size_t index = 0;
    for (auto& entry : entries)
    {
      if (std::holds_alternative<file_t>(entry))
      {
        const auto& file = std::get<file_t>(entry);
        checksum += file.file_number * index;
      }
      ++index;
    }
    return checksum;
  }

  template<typename variant_type, std::size_t N = 1>
  void find_next(auto& iterator, const auto& iterator_end)
  {
    while (iterator != iterator_end)
    {
      std::advance(iterator, N);
      if (std::holds_alternative<variant_type>(*iterator))
      {
        break;
      }
    }
  }

  auto find_fitting_free_space(entry_list_t& entries, std::uint8_t file_length)
  {
    bool has_room = false;
    auto iterator = std::begin(entries);
    std::optional<entry_list_t::iterator> start_of_free_space;
    std::uint8_t room_found = 0;
    for (; iterator != std::end(entries); ++iterator)
    {
      if (std::holds_alternative<free_space_t>(*iterator))
      {
        ++room_found;
        if (not start_of_free_space)
        {
          start_of_free_space = iterator;
        }
        if (room_found == file_length)
        {
          has_room = true;
          break;
        }
      }
      if (std::holds_alternative<file_t>(*iterator))
      {
        start_of_free_space.reset();
        room_found = 0;
        has_room = false;
      }
    }
    if (has_room and start_of_free_space)
    {
      return start_of_free_space.value();
    }
    return std::end(entries);
  }

  std::size_t part1(parse_result_t& result)
  {
    auto& [entries, last_file_index, first_free_space_index] = result;
    auto free_space_iterator = std::begin(entries);
    auto free_space_iterator_end = std::end(entries);
    auto file_iterator = std::rbegin(entries);
    auto file_iterator_end = std::rend(entries);

    std::advance(free_space_iterator, first_free_space_index);
    std::advance(file_iterator, entries.size() - last_file_index);
    while (free_space_iterator !=  free_space_iterator_end and
           file_iterator != file_iterator_end and
           std::distance(file_iterator, std::rend(entries)) > std::distance(std::begin(entries), free_space_iterator))
    {
      auto file_number_to_compress = std::get<file_t>(*file_iterator).file_number;
      while (free_space_iterator !=  free_space_iterator_end and
             file_iterator != file_iterator_end and
             std::get<file_t>(*file_iterator).file_number == file_number_to_compress)
      {
        std::swap(*file_iterator, *free_space_iterator);

        find_next<free_space_t>(free_space_iterator, free_space_iterator_end);
        find_next<file_t>(file_iterator, file_iterator_end);
      }
    }

    return checksum(entries);
  }

  std::size_t part2(parse_result_t& result)
  {
    auto& entries = std::get<0>(result);
    auto& last_file_index = std::get<1>(result);

    auto file_iterator = std::rbegin(entries);

    std::set<std::size_t> touched_files;
    std::advance(file_iterator, entries.size() - last_file_index);
    while (file_iterator != std::rend(entries))
    {
      auto file_number_to_compress = std::get<file_t>(*file_iterator).file_number;
      auto file_length = std::get<file_t>(*file_iterator).length;
      if (not touched_files.contains(file_number_to_compress))
      {
        auto free_space_iterator = find_fitting_free_space(entries, file_length);
        auto file_distance = std::distance(file_iterator + file_length, std::rend(entries));
        auto free_space_distance = std::distance(std::begin(entries), free_space_iterator);
        while (file_distance > free_space_distance and
               free_space_iterator != std::end(entries) and
               file_iterator != std::rend(entries) and
               std::get<file_t>(*file_iterator).file_number == file_number_to_compress)
        {
          std::swap(*file_iterator, *free_space_iterator);

          find_next<free_space_t>(free_space_iterator, std::end(entries));
          find_next<file_t>(file_iterator, std::rend(entries));
        }
        touched_files.emplace(file_number_to_compress);
      }
      else
      {
        find_next<file_t>(file_iterator, std::rend(entries));
      }
    }
    return checksum(entries);
  }

}

int main(int, char**)
{
  try
  {
    const input_t input = utility::read_file(std::filesystem::current_path() / "day09.txt"sv);
    auto parsed = parse(input[0]);
    auto parsed_part2 = parsed;
    std::string test2{"2333133121414131402"};
    auto parsed_test2 = parse(test2);
    utility::run_part(1, [](auto& p){ return part1(p); }, parsed);
    utility::run_part(2, [](auto& p){ return part2(p); }, parsed_test2);
    utility::run_part(2, [](auto& p){ return part2(p); }, parsed_part2);
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