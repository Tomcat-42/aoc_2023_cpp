#include <aoc/day1.hpp>

#include <functional>
#include <iostream>
#include <istream>
#include <numeric>
#include <print>
#include <ranges>
#include <regex>
#include <unordered_map>
#include <utility>

namespace aoc {
  auto day1(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>> {
    auto part1 = [&]() {
      auto extract_number = [](const std::string& s) {
        auto digits = s | std::views::filter(::isdigit);
        auto res = std::format("{}{}", digits.front(), digits.back());
        return std::stoi(res);
      };
      std::vector<int> numbers;
      for (const auto& str : std::ranges::istream_view<std::string>(input)) {
        numbers.push_back(extract_number(str));
      }

      int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
      return std::to_string(sum);
    };

    auto part2 = [&]() {
      auto token_map = std::unordered_map<std::string, std::string> {
        { "one", "1" },   { "two", "2" },   { "three", "3" },
        { "four", "4" },  { "five", "5" },  { "six", "6" },
        { "seven", "7" }, { "eight", "8" }, { "nine", "9" }
      };

      auto tokenize = [&](const std::string& s) {
        std::regex pattern(
          R"((?=(one|two|three|four|five|six|seven|eight|nine|\d)))");
        auto words_begin = std::sregex_iterator(s.begin(), s.end(), pattern);
        auto words_end = std::sregex_iterator();

        std::string first_match;
        std::string last_match;

        for (auto it = words_begin; it != words_end; ++it) {
          std::smatch match = *it;
          println("{}", match.str(1));
          if (first_match.empty()) {
            first_match = match.str(1);
          }
          last_match = match.str(1);
        }

        auto res = std::format(
          "{}{}",
          token_map.count(first_match) > 0 ? token_map[first_match]
                                           : first_match,
          token_map.count(last_match) > 0 ? token_map[last_match] : last_match);
        return std::stoi(res);
      };

      std::vector<int> numbers;
      for (const auto& str : std::ranges::istream_view<std::string>(input)) {
        numbers.push_back(tokenize(str));
      }
      int sum = std::accumulate(numbers.begin(), numbers.end(), 0);
      return std::to_string(sum);
    };

    return { part1, part2 };
  }
} // namespace aoc
