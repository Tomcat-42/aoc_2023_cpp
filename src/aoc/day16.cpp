#include <algorithm>
#include <aoc/day16.hpp>

#include <functional>
#include <istream>
#include <print>
#include <queue>
#include <ranges>
#include <set>
#include <utility>
#include <vector>

namespace aoc::day16 {
  auto contraption::from_lines(std::vector<std::string> const& lines)
    -> contraption {

    auto tiles =
      lines | std::views::transform([](auto const& line) {
        return line | std::views::transform([](auto const& c) {
                 return std::make_pair(static_cast<tile>(c), 0ull);
               }) |
          std::ranges::to<std::vector<std::pair<tile, std::uint64_t>>>();
      }) |
      std::ranges::to<std::vector>();
    return contraption { .tiles = tiles };
  }

  auto contraption::beam(position p, direction d) -> decltype(*this) {
    auto next_tiles =
      [this](position p,
             direction d) -> std::vector<std::pair<position, direction>> {
      auto [i, j] = p;
      std::vector<std::pair<position, direction>> next;

      switch (d) {
        case direction::right:
          switch (tiles[i][j].first) {
            case tile::empty:
            case tile::splitter_horizontal:
              if (j + 1 < tiles[i].size())
                next.push_back({ { i, j + 1 }, direction::right });
              break;
            case tile::splitter_vertical:
              if (i + 1 < tiles.size())
                next.push_back({ { i + 1, j }, direction::down });
              if (i > 0)
                next.push_back({ { i - 1, j }, direction::up });
              break;
            case tile::mirror_up:
              if (i > 0)
                next.push_back({ { i - 1, j }, direction::up });
              break;
            case tile::mirror_down:
              if (i + 1 < tiles.size())
                next.push_back({ { i + 1, j }, direction::down });
              break;
          }

          break;
        case direction::left:
          switch (tiles[i][j].first) {
            case tile::empty:
            case tile::splitter_horizontal:
              if (j > 0)
                next.push_back({ { i, j - 1 }, d });
              break;
            case tile::splitter_vertical:
              if (i + 1 < tiles.size())
                next.push_back({ { i + 1, j }, direction::down });
              if (i > 0)
                next.push_back({ { i - 1, j }, direction::up });
              break;
            case tile::mirror_down:
              if (i > 0)
                next.push_back({ { i - 1, j }, direction::up });
              break;
            case tile::mirror_up:
              if (i + 1 < tiles.size())
                next.push_back({ { i + 1, j }, direction::down });
              break;
          }
          break;
        case direction::up:
          switch (tiles[i][j].first) {
            case tile::empty:
            case tile::splitter_vertical:
              if (i > 0)
                next.push_back({ { i - 1, j }, direction::up });
              break;

            case tile::splitter_horizontal:
              if (j + 1 < tiles[i].size())
                next.push_back({ { i, j + 1 }, direction::right });
              if (j > 0)
                next.push_back({ { i, j - 1 }, direction::left });
              break;
            case tile::mirror_up:
              if (j + 1 < tiles[i].size())
                next.push_back({ { i, j + 1 }, direction::right });
              break;
            case tile::mirror_down:
              if (j > 0)
                next.push_back({ { i, j - 1 }, direction::left });
              break;
          }
          break;

        case direction::down:
          switch (tiles[i][j].first) {
            case tile::empty:
            case tile::splitter_vertical:
              if (i + 1 < tiles.size())
                next.push_back({ { i + 1, j }, direction::down });
              break;
            case tile::splitter_horizontal:
              if (j + 1 < tiles[i].size())
                next.push_back({ { i, j + 1 }, direction::right });
              if (j > 0)
                next.push_back({ { i, j - 1 }, direction::left });
              break;
            case tile::mirror_down:
              if (j + 1 < tiles[i].size())
                next.push_back({ { i, j + 1 }, direction::right });
              break;
            case tile::mirror_up:
              if (j > 0)
                next.push_back({ { i, j - 1 }, direction::left });
              break;
          }
          break;
      }

      return next;
    };

    std::queue<std::pair<position, direction>> q;
    std::set<std::pair<position, direction>> visited;

    q.emplace(p, d);
    while (!q.empty()) {
      auto vertex = q.front();
      visited.insert(vertex);
      q.pop();

      auto [p, d] = vertex;
      auto [i, j] = p;
      auto& [tile, count] = tiles[i][j];
      count++;

      auto next = next_tiles(p, d);
      for (auto v : next) {
        if (!visited.contains(v))
          q.push(v);
      }
    }

    return *this;
  }

  auto contraption::eval() const -> std::uint64_t {
    auto count = 0ull;
    for (auto const& row : tiles)
      for (auto const& [tile, c] : row)
        if (c > 0)
          count++;
    return count;
  }

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {
    auto lines =
      std::views::istream<std::string>(input) | std::ranges::to<std::vector>();

    auto const contraption = contraption::from_lines(lines);

    const auto pt1 = [=]() mutable -> std::string {
      auto c = contraption;
      return std::to_string(
        c.beam({ 0, 0 }, contraption::direction::right).eval());
    };

    const auto pt2 = [=]() -> std::string {
      std::vector<std::uint64_t> values;
      // top row
      for (auto j = 0ull; j < contraption.tiles[0].size(); ++j) {
        auto c = contraption;
        values.push_back(c.beam({ 0, j }, contraption::direction::down).eval());
      }

      // bottom row
      for (auto j = 0ull; j < contraption.tiles[0].size(); ++j) {
        auto c = contraption;
        values.push_back(c.beam({ contraption.tiles.size() - 1, j },
                                contraption::direction::up)
                           .eval());
      }

      // left column
      for (auto i = 0ull; i < contraption.tiles.size(); ++i) {
        auto c = contraption;
        values.push_back(
          c.beam({ i, 0 }, contraption::direction::right).eval());
      }

      // right column
      for (auto i = 0ull; i < contraption.tiles.size(); ++i) {
        auto c = contraption;
        values.push_back(c.beam({ i, contraption.tiles[i].size() - 1 },
                                contraption::direction::left)
                           .eval());
      }

      auto max = std::ranges::max(values);

      return std::to_string(max);
    };

    return { pt1, pt2 };
  }
} // namespace aoc::day16

namespace std {

  template <typename FormatContext>
  auto formatter<contraption>::format(const contraption& m,
                                      FormatContext& ctx) const
    -> decltype(ctx.out()) {
    auto out = ctx.out();

    out = format_to(out, "\033[34m  ");
    for (size_t col = 0; col < m.tiles[0].size(); ++col) {
      out = format_to(out, "{:1}", col);
    }
    out = format_to(out, "\033[0m\n");

    for (size_t row = 0; row < m.tiles.size(); ++row) {
      out = format_to(out, "\033[34m{:1} \033[0m", row);
      for (auto col : m.tiles[row])
        out = format_to(out, "{:1}",
                        col.second > 0ull ? "#" : std::format("{}", col.first));

      out = format_to(out, "\n");
    }

    return out;
  }

  template <typename FormatContext>
  auto formatter<contraption::tile>::format(const contraption::tile& pt,
                                            FormatContext& ctx) const
    -> decltype(ctx.out()) {
    std::string color;
    switch (pt) {
      case contraption::tile::empty:
        color = "\033[37m"; // White
        break;
      case contraption::tile::mirror_up:
        color = "\033[34m"; // Blue
        break;
      case contraption::tile::mirror_down:
        color = "\033[32m"; // Green
        break;
      case contraption::tile::splitter_vertical:
        color = "\033[31m"; // Red
        break;
      case contraption::tile::splitter_horizontal:
        color = "\033[33m"; // Yellow
        break;
    }

    return format_to(ctx.out(), "{}{:1}\033[0m", color, static_cast<char>(pt));
  }
} // namespace std
