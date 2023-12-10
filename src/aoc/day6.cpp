#include <aoc/day6.hpp>

#include <functional>
#include <iostream>
#include <istream>
#include <numeric>
#include <ranges>
#include <regex>
#include <sstream>
#include <utility>

namespace aoc::day6 {
  using std::regex;

  struct Races {
    std::vector<size_t> times;
    std::vector<size_t> distances;
    static auto from_str(const std::string&&) -> const Races;
  };

  auto Races::from_str(const std::string&& str) -> const Races {
    auto stream { std::istringstream { std::string { str } } };
    std::string line;

    std::vector<size_t> times;
    std::vector<size_t> distances;

    std::getline(stream, line, ':');
    std::getline(stream, line);
    auto time_str = std::istringstream { line };
    std::ranges::copy(std::views::istream<size_t>(time_str),
                      std::back_inserter(times));

    std::getline(stream, line, ':');
    std::getline(stream, line);
    auto distance_str = std::istringstream { line };
    std::ranges::copy(std::views::istream<size_t>(distance_str),
                      std::back_inserter(distances));

    return { .times = times, .distances = distances };
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>> {

    const auto str = std::string { std::istreambuf_iterator<char> { input },
                                   std::istreambuf_iterator<char> {} };
    const auto compute = [](Races problem) {
      auto records =
        std::views::zip(problem.times, problem.distances) |
        std::views::transform([](const auto& pair) {
          auto records = std::views::iota(0ull, std::get<0>(pair)) |
            std::views::transform([&](const auto i) {
                           return i * (std::get<0>(pair) - i);
                         }) |
            std::views::filter([&](auto i) { return i > std::get<1>(pair); });
          return std::ranges::distance(records);
        });
      return std::accumulate(std::begin(records), std::end(records), 1ull,
                             std::multiplies<>());
    };

    const auto pt1 = [=]() -> const std::string {
      const auto problem = Races::from_str(std::move(str));
      return std::to_string(compute(problem));
    };

    const auto pt2 = [=]() -> const std::string {
      auto problem =
        Races::from_str(std::regex_replace(str, std::regex(" "), ""));
      return std::to_string(compute(problem));
    };

    return { pt1, pt2 };
  }
} // namespace aoc::day6
