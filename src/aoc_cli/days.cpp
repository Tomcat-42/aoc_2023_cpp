#include <aoc_cli/days.hpp>
#include <map>

namespace aoc_cli {
  const std::map<
    int,
    std::function<std::pair<std::function<std::string()>,
                            std::function<std::string()>>(std::istream&)>>
    days = { { 1, aoc::day1 } };

} // namespace aoc_cli
