#include <algorithm>
#include <aoc/day14.hpp>

#include <compare>
#include <functional>
#include <istream>
#include <map>
#include <print>
#include <ranges>
#include <utility>
#include <vector>

namespace aoc::day14 {
  auto platform::transpose_grid() -> void {
    std::vector<std::vector<tile_type>> new_grid(
      grid[0].size(), std::vector<tile_type>(grid.size()));

    for (std::size_t i = 0; i < grid.size(); ++i)
      for (std::size_t j = 0; j < grid[i].size(); ++j)
        new_grid[j][i] = grid[i][j];

    grid = std::move(new_grid);
  }

  auto platform::tilt(direction d) -> decltype(*this) {
    auto sort_line = [](std::vector<tile_type>& line) {
      std::size_t last_move_index = 0;
      for (std::size_t j = 0; j < line.size(); ++j) {
        if (line[j] == tile_type::move) {
          if (j != last_move_index) {
            std::swap(line[j], line[last_move_index]);
          }
          ++last_move_index;
        } else if (line[j] == tile_type::not_move) {
          last_move_index = j + 1;
        }
      }
    };

    switch (d) {
      case direction::up:
        transpose_grid();
        std::ranges::for_each(
          grid, [&](std::vector<tile_type>& line) { sort_line(line); });
        transpose_grid();
        break;
      case direction::right:
        std::ranges::for_each(grid, [&](std::vector<tile_type>& line) {
          std::ranges::reverse(line);
          sort_line(line);
          std::ranges::reverse(line);
        });
        break;
      case direction::left:
        std::ranges::for_each(
          grid, [&](std::vector<tile_type>& line) { sort_line(line); });
        break;
      case direction::down:
        transpose_grid();
        std::ranges::for_each(grid, [&](std::vector<tile_type>& line) {
          std::ranges::reverse(line);
          sort_line(line);

          std::ranges::reverse(line);
        });
        transpose_grid();
        break;

        break;
      default:
        break;
    }
    return *this;
  }

  auto platform::count() -> std::uint64_t {
    auto count = 0ll;
    for (auto i = 0ll; i < grid.size(); ++i)
      count += std::ranges::count(grid[i], tile_type::move) * (grid.size() - i);
    return count;
  }

  auto operator<=>(platform const& lhs, platform const& rhs)
    -> std::partial_ordering {
    return lhs.grid <=> rhs.grid;
  }

  auto platform::from_lines(const std::vector<std::string>& lines) -> platform {
    auto grid = lines | std::views::transform([](auto const& line) {
                  return line | std::views::transform([](auto const& c) {
                           return static_cast<tile_type>(c);
                         }) |
                    std::ranges::to<std::vector>();
                }) |
      std::ranges::to<std::vector>();
    return { std::move(grid) };
  }

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {
    auto lines =
      std::views::istream<std::string>(input) | std::ranges::to<std::vector>();
    auto grid = platform::from_lines(lines);
    const auto pt1 = [=]() mutable -> std::string {
      return std::to_string(grid.tilt(platform::direction::up).count());
    };

    const auto pt2 = [=]() mutable -> std::string {
      constexpr auto iterations = 1000000000;
      std::map<platform, std::uint64_t> seen;
      for (auto i = 0ull; i < iterations; ++i) {
        // search for a cycle
        auto it = seen.find(grid);
        if (it != seen.end()) {
          auto const& [_, index] = *it;
          for (auto j = (iterations - index) % (i - index); j > 0; --j) {
            grid.tilt(platform::direction::up)
              .tilt(platform::direction::left)
              .tilt(platform::direction::down)
              .tilt(platform::direction::right);
          }
          break;
        }

        seen.try_emplace(grid, i);
        grid.tilt(platform::direction::up)
          .tilt(platform::direction::left)
          .tilt(platform::direction::down)
          .tilt(platform::direction::right);
      };
      return std::to_string(grid.count());
    };
    return { pt1, pt2 };
  }
} // namespace aoc::day14

auto operator<=>(aoc::day14::platform::tile_type const& lhs,
                 aoc::day14::platform::tile_type const& rhs)

  -> std::partial_ordering {
  if (lhs == aoc::day14::platform::tile_type::move &&
      rhs == aoc::day14::platform::tile_type::empty) {
    return std::partial_ordering::less;
  }
  return std::partial_ordering::equivalent;
}

namespace std {
  template <typename FormatContext>
  auto formatter<aoc::day14::platform::tile_type>::format(
    const aoc::day14::platform::tile_type& pt, FormatContext& ctx) const
    -> decltype(ctx.out()) {
    std::string color;
    switch (pt) {
      case aoc::day14::platform::tile_type::not_move:
        color = "\033[31m";
        break; // Red
      case aoc::day14::platform::tile_type::move:
        color = "\033[32m";
        break; // Green
      case aoc::day14::platform::tile_type::empty:
        color = "\033[33m";
        break; // Yellow
      default:
        color = "\033[0m";
    }

    return format_to(ctx.out(), "{}{:3}\033[0m", color, static_cast<char>(pt));
  }
  template <typename FormatContext>
  auto formatter<aoc::day14::platform>::format(const aoc::day14::platform& m,
                                               FormatContext& ctx) const
    -> decltype(ctx.out()) {
    auto out = ctx.out();

    out = format_to(out, "\033[34m  ");
    for (size_t col = 0; col < m.grid[0].size(); ++col) {
      out = format_to(out, "{:3}", col);
    }
    out = format_to(out, "\033[0m\n");

    for (size_t row = 0; row < m.grid.size(); ++row) {
      out = format_to(out, "\033[34m{:3} \033[0m", row);
      for (auto col : m.grid[row])
        out = format_to(out, "{}", col);
      out = format_to(out, "\n");
    }

    return out;
  }

} // namespace std
