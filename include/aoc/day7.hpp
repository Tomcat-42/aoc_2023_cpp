#pragma once

#include <functional>
#include <istream>
#include <utility>

namespace aoc::day7 {
  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;
} // namespace aoc::dayn
