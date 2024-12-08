#pragma once

#include <algorithm>
#include <charconv>
#include <execution>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <optional>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <fmt/format.h>
#include <fmt/compile.h>
#include <scn/scan.h>

using input_t = std::vector<std::string>;

namespace utility
{
  inline input_t read_file(const std::filesystem::path& path)
  {
    input_t input;
    std::ifstream file_in(path);
    if (!file_in)
    {
      throw std::runtime_error(fmt::format(FMT_STRING("Unable to open input file: {0}"), path.string()));
    }

    std::string line;
    while (std::getline(file_in, line))
    {
      input.push_back(line);
    }
    return input;
  }

  inline void left_trim(std::string &s)
  {
    s.erase(s.begin(), std::ranges::find_if(s, [](unsigned char ch) {
      return !std::isspace(ch);
    }));
  }

  // trim from end (in place)
  inline void right_trim(std::string &s)
  {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
      return !std::isspace(ch);
    }).base(), s.end());
  }

  // trim from both ends (in place)
  inline void trim(std::string &s)
  {
    right_trim(s);
    left_trim(s);
  }

  void print_output(std::uint8_t part_no, auto& result, auto time_taken)
  {
    fmt::print(FMT_STRING("Part {}: {} in {}us\n"), part_no, result, time_taken);
  }

  template <typename...Args>
  auto run_part(std::uint8_t part_no, auto to_run, Args&&... args)
  {
    auto start = std::chrono::high_resolution_clock::now();
    auto result = to_run(std::forward<Args&&>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto time_taken = end - start;
    print_output(part_no, result, std::chrono::duration_cast<std::chrono::microseconds>(time_taken).count());
  }
}