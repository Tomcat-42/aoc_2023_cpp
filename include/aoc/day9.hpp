#pragma once

#include <functional>
#include <istream>
#include <utility>

namespace aoc::day9 {
  using report = std::vector<std::uint64_t>;

  auto interpolate(report const& r) -> std::uint64_t;

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;
} // namespace aoc::day9

namespace std {
  auto operator<<(std::ostream& os, aoc::day9::report& r) -> std::ostream&;
  auto operator>>(std::istream& is, aoc::day9::report& r) -> std::istream&;
} // namespace std
