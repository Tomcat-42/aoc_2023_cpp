#include <aoc/dayn.hpp>

#include <functional>
#include <istream>
#include <utility>

namespace aoc::dayn {
  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {
    const auto pt1 = []() -> std::string {
      return "not implemented yet";
    };

    const auto pt2 = []() -> std::string {
      return "not implemented yet";
    };

    return { pt1, pt2 };
  }
} // namespace aoc::dayn
