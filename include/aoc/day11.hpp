#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <istream>
#include <set>
#include <utility>

namespace aoc::day11 {

  struct universe {
    enum class point_type : char {
      empty = '.',
      galaxy = '#',
    };

    using graph = std::vector<std::vector<double>>;
    using node =
      std::tuple<std::uint64_t, std::pair<std::uint64_t, std::uint64_t>,
                 point_type>;

    std::set<std::uint64_t> empty_rows, empty_columns;
    std::vector<std::vector<point_type>> matrix;
    universe(std::vector<std::vector<point_type>>&& m);

    [[nodiscard]] auto floyd_warshall(std::uint64_t time_dilation = 2) const
      -> const std::uint64_t;
  };
  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;
} // namespace aoc::day11

namespace std {
  using universe = aoc::day11::universe;

  template <>
  struct std::formatter<universe> {

    template <typename FormatContext>
    constexpr auto parse(FormatContext& ctx)
      -> std::format_parse_context::iterator {
      return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const universe& m, FormatContext& ctx) const
      -> decltype(ctx.out());
  };

  template <>
  struct std::formatter<universe::point_type> {
    template <typename FormatContext>
    constexpr auto parse(FormatContext& ctx)
      -> std::format_parse_context::iterator {
      return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const universe::point_type& pt, FormatContext& ctx) const
      -> decltype(ctx.out());
  };
} // namespace std
