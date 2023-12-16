#pragma once

#include <functional>
#include <istream>
#include <string>
#include <utility>
#include <vector>

namespace aoc::day14 {
  struct platform {
    enum class tile_type : char {
      move = 'O',
      not_move = '#',
      empty = '.'
    };
    enum class direction {
      up,
      down,
      left,
      right
    };
    std::vector<std::vector<tile_type>> grid;
    auto transpose_grid() -> void;
    friend auto operator<=>(platform::tile_type const&,
                            platform::tile_type const&)
      -> std::partial_ordering;
    friend auto operator<=>(platform const&, platform const&)
      -> std::partial_ordering;
    static auto from_lines(std::vector<std::string> const&) -> platform;
    auto count() -> std::uint64_t;
    auto tilt(direction) -> decltype(*this);
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;
} // namespace aoc::day14

auto operator<=>(aoc::day14::platform::tile_type const& a,
                 aoc::day14::platform::tile_type const& b)

  -> std::partial_ordering;

namespace std {

  template <>
  struct std::formatter<aoc::day14::platform::tile_type> {

    template <typename FormatContext>
    constexpr auto parse(FormatContext& ctx)
      -> std::format_parse_context::iterator {
      return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const aoc::day14::platform::tile_type& pt,
                FormatContext& ctx) const -> decltype(ctx.out());
  };

  template <>
  struct std::formatter<aoc::day14::platform> {

    template <typename FormatContext>
    constexpr auto parse(FormatContext& ctx)
      -> std::format_parse_context::iterator {
      return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const aoc::day14::platform& pt, FormatContext& ctx) const

      -> decltype(ctx.out());
  };

} // namespace std
