#pragma once

#include <functional>
#include <istream>
#include <utility>

namespace aoc::dayn {
  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>>;
} // namespace aoc::dayn
