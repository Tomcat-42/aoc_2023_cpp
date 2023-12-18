#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <istream>
#include <map>
#include <ranges>
#include <regex>
#include <utility>

namespace aoc::day8 {
  enum class instruction : char {
    left = 'L',
    right = 'R',
  };

  auto operator>>(std::istream& is, aoc::day8::instruction& i) -> std::istream&;
  auto operator<<(std::ostream& os, aoc::day8::instruction i) -> std::ostream&;

  struct graph {
    std::map<std::string, std::pair<std::string, std::string>> nodes;
    using entry = std::pair<std::string, std::pair<std::string, std::string>>;

    static auto from_entries(std::ranges::range auto&& entries) -> graph;
    [[nodiscard]] auto traverse(std::ranges::range auto&& instructions,
                                std::regex begin, std::regex end) const
      -> std::uint64_t;
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;

} // namespace aoc::day8

namespace std {
  auto operator>>(std::istream& is, aoc::day8::graph::entry& e)
    -> std::istream&;

  auto operator<<(std::ostream& os, aoc::day8::graph::entry e) -> std::ostream&;

  template <>
  struct std::formatter<aoc::day8::graph> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const aoc::day8::graph& g, FormatContext& ctx) const {
      return std::format_to(ctx.out(), "{}", g.nodes);
    }
  };

} // namespace std
