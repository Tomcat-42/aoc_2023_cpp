#include <aoc/dayn.hpp>

#include <functional>
#include <istream>
#include <utility>

namespace aoc::dayn {
  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>> {
    return { []() -> std::string { return "not implemented yet"; },
             []() -> std::string {
               return "not implemented yet";
             } };
  }
} // namespace aoc::dayn
