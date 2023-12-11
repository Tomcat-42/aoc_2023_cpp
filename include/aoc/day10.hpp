#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <istream>
#include <map>
#include <ranges>
#include <set>
#include <utility>
#include <vector>

namespace aoc::day10 {
  struct maze {
    enum class tile_type {
      up_down,
      left_right,
      up_right,
      up_left,
      left_down,
      right_down,
      empty,
      start,
      debug
    };
    enum class direction : char {
      up = 'U',
      down = 'D',
      left = 'L',
      right = 'R',
      start = 'S'
    };
    using position = std::pair<std::uint64_t, std::uint64_t>;

    static const std::map<char, tile_type> tile_map;
    static const std::map<tile_type, char> char_map;

    static auto parse(std::istream&) -> const maze;
    static auto deduce_type(std::array<tile_type, 4>) -> const tile_type;

    [[nodiscard]] auto connected_vertices(position) const
      -> std::vector<position>;
    // [[nodiscard]] auto get_direction(position from, position to) const
    //   -> direction;
    [[nodiscard]] auto depth() const -> std::uint64_t;
    [[nodiscard]] auto loop() const -> std::set<position>;
    [[nodiscard]] auto connected_area() const -> std::uint64_t;

    std::vector<std::vector<tile_type>> matrix;
    position start;
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;
} // namespace aoc::day10

namespace std {
  using maze = aoc::day10::maze;
  using tile_type = aoc::day10::maze::tile_type;
  using direction = aoc::day10::maze::direction;
  using position = aoc::day10::maze::position;

  template <>
  struct std::formatter<tile_type> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }
    template <typename FormatContext>
    auto format(tile_type t, FormatContext& ctx) const {
      return std::formatter<char>().format(maze::char_map.at(t), ctx);
    }
  };

  template <>
  struct std::formatter<maze> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const maze& m, FormatContext& ctx) const {
      auto out = ctx.out();

      out = std::format_to(out, "  ");
      for (size_t col = 0; col < m.matrix[0].size(); ++col) {
        out = std::formatter<size_t>().format(col, ctx);
      }
      out = std::format_to(out, "\n");

      auto path = m.loop() | std::ranges::to<std::set<position>>();

      // Print the matrix with row indexes
      for (size_t row = 0; row < m.matrix.size(); ++row) {
        out = std::formatter<size_t>().format(row, ctx);
        out = std::format_to(out, " ");
        for (size_t col = 0; col < m.matrix[row].size(); ++col) {
          auto tile =
            path.contains({ row, col }) ? m.matrix[row][col] : tile_type::debug;

          out = std::formatter<tile_type>().format(tile, ctx);
        }
        out = std::format_to(out, "\n");
      }

      return out;
    }
  };
} // namespace std
