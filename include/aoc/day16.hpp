#pragma once

#include <functional>
#include <istream>
#include <utility>

namespace aoc::day16 {
  struct contraption {
    enum class tile : char {
      empty = '.',
      mirror_up = '/',
      mirror_down = '\\',
      splitter_vertical = '|',
      splitter_horizontal = '-',
    };

    enum class direction : char {
      up = '^',
      down = 'v',
      left = '<',
      right = '>',
    };

    using position = std::pair<std::uint64_t, std::uint64_t>;

    std::vector<std::vector<std::pair<tile, std::uint64_t>>> tiles;
    static auto from_lines(std::vector<std::string> const& lines)
      -> contraption;
    auto beam(position p, direction d) -> decltype(*this);
    [[nodiscard]] auto eval() const -> std::uint64_t;
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;
} // namespace aoc::day16

namespace std {
  using contraption = aoc::day16::contraption;

  template <>
  struct std::formatter<contraption> {

    template <typename FormatContext>
    constexpr auto parse(FormatContext& ctx)
      -> std::format_parse_context::iterator {
      return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const contraption& m, FormatContext& ctx) const
      -> decltype(ctx.out());
  };

  template <>
  struct std::formatter<contraption::tile> {
    template <typename FormatContext>
    constexpr auto parse(FormatContext& ctx)
      -> std::format_parse_context::iterator {

      return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const contraption::tile& pt, FormatContext& ctx) const
      -> decltype(ctx.out());
  };
} // namespace std
