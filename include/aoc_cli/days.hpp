#pragma once
#include <aoc/day1.hpp>

#include <functional>
#include <istream>
#include <map>

namespace aoc_cli {

  extern const std::map<
    int,
    std::function<std::pair<std::function<std::string()>,
                            std::function<std::string()>>(std::istream&)>>
    days;

} // namespace aoc_cli
