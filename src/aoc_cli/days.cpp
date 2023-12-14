#include <aoc_cli/days.hpp>

#include <aoc/day1.hpp>
#include <aoc/day10.hpp>
#include <aoc/day11.hpp>
#include <aoc/day12.hpp>
#include <aoc/day13.hpp>
#include <aoc/day2.hpp>
#include <aoc/day3.hpp>
#include <aoc/day4.hpp>
#include <aoc/day5.hpp>
#include <aoc/day6.hpp>
#include <aoc/day7.hpp>
#include <aoc/day8.hpp>
#include <aoc/day9.hpp>
#include <aoc/dayn.hpp>

#include <map>

namespace aoc_cli {
  const std::map<
    int,
    std::function<std::pair<std::function<std::string()>,
                            std::function<std::string()>>(std::istream&)>>
    days = { { 1, aoc::day1::solution },   { 2, aoc::day2::solution },
             { 3, aoc::day3::solution },   { 4, aoc::day4::solution },
             { 5, aoc::day5::solution },   { 6, aoc::day6::solution },
             { 7, aoc::day7::solution },   { 8, aoc::day8::solution },
             { 9, aoc::day9::solution },   { 10, aoc::day10::solution },
             { 11, aoc::day11::solution }, { 12, aoc::day12::solution },
             { 13, aoc::day13::solution }, { 25, aoc::dayn::solution } };

} // namespace aoc_cli
