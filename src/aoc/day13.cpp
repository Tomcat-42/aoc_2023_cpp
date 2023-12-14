#include <aoc/day13.hpp>

#include <cstdint>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <utility>

namespace aoc::day13 {

  [[nodiscard]] auto pattern::find_reflections() const
    -> std::pair<std::uint64_t, std::uint64_t> {
    auto num = 0ull;
    auto sum = 0ull;

    auto width = lines.empty() ? 0 : lines[0].size();
    for (auto j = 0ull; j < width - 1; ++j) {
      auto is_to_sum = true;
      for (auto k = std::min(j + 1, width - j - 1); k > 0; --k) {
        for (const auto& line : lines) {
          if (line[j - k + 1] != line[j + k]) {
            is_to_sum = false;
            goto sum;
          }
        }
      }
sum:
      if (is_to_sum) {
        num += 1;
        sum += (j + 1);
      }
    }
    for (auto i = 0ull; i < lines.size() - 1; ++i) {
      auto is_to_sum = true;
      for (auto k = std::min(i + 1, lines.size() - i - 1); k > 0; --k) {
        if (lines[i - k + 1] != lines[i + k]) {
          is_to_sum = false;
          break;
        }
      }
      if (is_to_sum) {
        num += 1;
        sum += 100 * (i + 1);
      }
    }

    return { sum, num };
  }

  auto pattern::flip_all_and_find_different_reflection() -> std::uint64_t {
    auto flip = [](char& c) {
      switch (c) {
        case '#':
          c = '.';
          break;
        case '.':
          c = '#';
          break;
        default:
          break;
      }
    };

    auto [original_reflections_sum, original_reflections_num] =
      find_reflections();

    auto width = lines.empty() ? 0 : lines[0].size();
    auto height = lines.size();
    for (auto i = 0ull; i < height; ++i) {
      for (auto j = 0ull; j < width; ++j) {
        flip(lines[i][j]);
        auto [reflection_sum, reflection_num] = find_reflections();
        std::println("original_reflections_num: {}, reflection_num: {}",
                     original_reflections_num, reflection_num);
        // case 1
        if ((original_reflections_num == 0 && reflection_num == 1) ||
            (original_reflections_num == 1 && reflection_num == 1)) {
          std::println("lines[i][j]: {}", lines[i][j]);
          std::println("original_reflections_sum: {}, reflection_sum: {}",
                       original_reflections_sum, reflection_sum);

          std::println("i: {}, j: {}", i, j);
          return reflection_sum;
        } else if (original_reflections_num == 1 && reflection_num == 2) {
          std::println("original_reflections_sum: {}, reflection_sum: {}",
                       original_reflections_sum, reflection_sum);

          return std::abs(static_cast<int>(reflection_sum) -
                          static_cast<int>(original_reflections_sum));
        }

        flip(lines[i][j]);
      }
    }

    std::cout << "Original pattern: \n" << original_reflections_num << '\n';
    return original_reflections_sum;
  }

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {
    auto patterns = std::views::istream<pattern>(input) |
      std::ranges::to<std::vector<pattern>>();

    const auto pt1 = [=]() -> std::string {
      std::ranges::copy(patterns, std::ostream_iterator<pattern>(std::cout));
      std::println("patterns: {}", patterns.size());
      auto sum = std::accumulate(std::begin(patterns), std::end(patterns), 0ull,
                                 [](auto acc, auto const& p) {
                                   return acc + p.find_reflections().first;
                                 });

      return std::to_string(sum);
    };

    const auto pt2 = [=]() mutable -> std::string {
      auto sum = std::accumulate(std::begin(patterns), std::end(patterns), 0ull,
                                 [](auto acc, auto& p) mutable {
                                   return acc +
                                     p.flip_all_and_find_different_reflection();
                                 });
      return std::to_string(sum);
    };

    return { pt1, pt2 };
  }

  auto operator>>(std::istream& input, pattern& p) -> std::istream& {
    p.lines.clear();
    std::string line;
    while (std::getline(input, line) && !line.empty()) {
      p.lines.push_back(std::move(line));
    }

    // input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input >> std::ws;
  }

  auto operator<<(std::ostream& output, pattern const& p) -> std::ostream& {
    std::ranges::copy(p.lines,
                      std::ostream_iterator<std::string>(output, "\n"));
    return output;
  }
} // namespace aoc::day13
