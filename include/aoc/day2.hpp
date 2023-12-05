#pragma once

#include <functional>
#include <istream>
#include <utility>

namespace aoc::day2 {
  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>>;
} // namespace aoc::day2
