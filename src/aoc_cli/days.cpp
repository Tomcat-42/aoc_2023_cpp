#include <aoc_cli/days.hpp>

#include <aoc/day1.hpp>
#include <aoc/day2.hpp>
#include <aoc/day3.hpp>
#include <aoc/day4.hpp>
#include <aoc/day5.hpp>
#include <aoc/dayn.hpp>

#include <map>

namespace aoc_cli {
  const std::map<
    int,
    std::function<std::pair<std::function<std::string()>,
                            std::function<std::string()>>(std::istream&)>>
    days = { { 1, aoc::day1::solution }, { 2, aoc::day2::solution },
             { 3, aoc::day3::solution }, { 4, aoc::day4::solution },
             { 5, aoc::day5::solution }, { 25, aoc::dayn::solution } };

} // namespace aoc_cli
