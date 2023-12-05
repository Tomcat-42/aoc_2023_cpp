#include <aoc_cli/days.hpp>

#include <aoc/day1.hpp>
#include <aoc/day2.hpp>

#include <map>

namespace aoc_cli {
  const std::map<
    int,
    std::function<std::pair<std::function<std::string()>,
                            std::function<std::string()>>(std::istream&)>>
    days = { { 1, aoc::day1::solution }, { 2, aoc::day2::solution } };

} // namespace aoc_cli
