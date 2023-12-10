#include <aoc/day9.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <sstream>
#include <utility>
#include <vector>

namespace aoc::day9 {
  auto interpolate(report const& r) -> std::uint64_t {
    auto result = std::vector<std::uint64_t> {};
    std::adjacent_difference(std::begin(r), std::end(r),
                             std::back_inserter(result));
    result.erase(std::begin(result));
    auto all_zero =
      !std::ranges::any_of(result, [](auto const& x) { return x != 0; });

    return r.back() + (all_zero ? 0 : interpolate(result));
  }

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {
    auto reports = std::views::istream<report>(input) |
      std::ranges::to<std::vector<report>>();

    const auto pt1 = [=]() -> std::string {
      auto interpolated = reports | std::views::transform(interpolate) |
        std::ranges::to<std::vector<std::uint64_t>>();
      auto sum =
        std::accumulate(std::begin(interpolated), std::end(interpolated), 0ULL,
                        [](auto const& acc, auto const& x) { return acc + x; });
      return std::to_string(sum);
    };

    const auto pt2 = [=]() -> std::string {
      auto interpolated_reverse = reports |
        std::views::transform([](auto const& r) {
                                    auto result = r;
                                    std::ranges::reverse(result);
                                    return interpolate(result);
                                  }) |
        std::ranges::to<std::vector<std::uint64_t>>();
      auto sum = std::accumulate(
        std::begin(interpolated_reverse), std::end(interpolated_reverse), 0ULL,
        [](auto const& acc, auto const& x) { return acc + x; });
      return std::to_string(sum);
    };

    return { pt1, pt2 };
  }
} // namespace aoc::day9

namespace std {
  auto operator>>(std::istream& is, aoc::day9::report& r) -> std::istream& {
    r.clear();
    std::string line;
    std::getline(is, line);
    std::istringstream iss { line };

    std::ranges::copy(std::istream_iterator<std::uint64_t> { iss },
                      std::istream_iterator<std::uint64_t> {},
                      std::back_inserter(r));
    return is;
  }

  auto operator<<(std::ostream& os, aoc::day9::report& r) -> std::ostream& {
    std::ranges::copy(r, std::ostream_iterator<std::uint64_t> { os, " " });
    return os;
  }
} // namespace std
