#pragma once

#include <functional>
#include <istream>
#include <utility>

namespace aoc {
  auto day1(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>>;
} // namespace aoc
