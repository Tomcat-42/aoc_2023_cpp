#include <algorithm>
#include <aoc/day11.hpp>

#include <cstdint>
#include <functional>
#include <istream>
#include <print>
#include <ranges>
#include <utility>
#include <vector>

namespace aoc::day11 {

  universe::universe(std::vector<std::vector<point_type>>&& matrix) {
    for (std::uint64_t i = 0; i < matrix.size(); ++i)
      if (std::ranges::all_of(matrix[i],
                              [](auto pt) { return pt == point_type::empty; }))
        empty_rows.insert(i);

    for (std::uint64_t j = 0; j < matrix[0].size(); ++j)
      if (std::ranges::all_of(
            matrix, [j](auto row) { return row[j] == point_type::empty; }))
        empty_columns.insert(j);

    this->matrix = std::move(matrix);
  }

  auto universe::floyd_warshall(std::uint64_t time_dilation) const
    -> const std::uint64_t {
    auto count_elements_in_interval =
      [](std::pair<std::uint64_t, std::uint64_t> p,
         const std::set<std::uint64_t>& s) {
        auto [x1, x2] = p;
        auto start = std::min(x1, x2);
        auto end = std::max(x1, x2);
        auto it = s.upper_bound(start);
        std::uint64_t count = 0;

        while (it != s.end() && *it < end) {
          ++count;
          ++it;
        }

        return count;
      };
    std::println("empty_rows: {}", empty_rows);
    std::println("empty_columns: {}", empty_columns);
    std::println("count {}",
                 count_elements_in_interval(std::make_pair(0, 0), empty_rows));

    auto galaxy_nodes = std::vector<node> {};
    for (size_t row = 0, idx = 0; row < matrix.size(); ++row)
      for (size_t col = 0; col < matrix[row].size(); ++col)
        if (matrix[row][col] == point_type::galaxy)
          galaxy_nodes.emplace_back(idx++, std::make_pair(row, col),
                                    point_type::galaxy);

    auto number_of_nodes = galaxy_nodes.size();
    auto g = graph(number_of_nodes,
                   std::vector<double>(
                     number_of_nodes, std::numeric_limits<double>::infinity()));

    for (auto i = 0; i < number_of_nodes; ++i)
      for (auto j = i + 1; j < number_of_nodes; ++j)

        g[i][j] = g[j][i] =
          std::abs(static_cast<double>(std::get<1>(galaxy_nodes[i]).first) -
                   static_cast<double>(std::get<1>(galaxy_nodes[j]).first)) +
          std::abs(static_cast<double>(std::get<1>(galaxy_nodes[i]).second) -
                   static_cast<double>(std::get<1>(galaxy_nodes[j]).second)) +
          static_cast<double>(time_dilation - 1) *
            (static_cast<double>(count_elements_in_interval(
               std::make_pair(std::get<1>(galaxy_nodes[i]).first,
                              std::get<1>(galaxy_nodes[j]).first),
               empty_rows)) +
             static_cast<double>(count_elements_in_interval(
               std::make_pair(std::get<1>(galaxy_nodes[i]).second,
                              std::get<1>(galaxy_nodes[j]).second),
               empty_columns)));

    for (auto k = 0; k < number_of_nodes; ++k)
      for (auto i = 0; i < number_of_nodes; ++i)
        for (auto j = 0; j < number_of_nodes; ++j)
          g[i][j] = std::min(g[i][j], g[i][k] + g[k][j]);

    auto sum = 0.0;
    for (auto i = 0; i < number_of_nodes; ++i)
      for (auto j = i + 1; j < number_of_nodes; ++j)
        sum += g[i][j];

    return sum;
  }

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {

    auto lines = std::views::istream<std::string>(input) |
      std::ranges::to<std::vector<std::string>>();
    auto matrix = lines | std::views::transform([](const std::string& line) {
                    return line | std::views::transform([](char c) {
                             return static_cast<universe::point_type>(c);
                           }) |
                      std::ranges::to<std::vector<universe::point_type>>();
                  }) |
      std::ranges::to<std::vector<std::vector<universe::point_type>>>();
    auto galaxies = universe { std::move(matrix) };

    const auto pt1 = [=]() -> std::string {
      return std::to_string(galaxies.floyd_warshall());
    };

    const auto pt2 = [=]() -> std::string {
      return std::to_string(galaxies.floyd_warshall(1'000'000));
    };

    return { pt1, pt2 };
  }
} // namespace aoc::day11

namespace std {
  template <typename FormatContext>
  auto formatter<universe>::format(const universe& m, FormatContext& ctx) const
    -> decltype(ctx.out()) {
    auto out = ctx.out();

    out = format_to(out, "\033[34m  ");
    for (size_t col = 0; col < m.matrix[0].size(); ++col) {
      out = format_to(out, "{:3}", col);
    }
    out = format_to(out, "\033[0m\n");

    for (size_t row = 0; row < m.matrix.size(); ++row) {
      out = format_to(out, "\033[34m{:3} \033[0m", row);
      for (auto col : m.matrix[row])
        out = format_to(out, "{}", col);
      out = format_to(out, "\n");
    }

    return out;
  }

  template <typename FormatContext>
  auto formatter<universe::point_type>::format(const universe::point_type& pt,
                                               FormatContext& ctx) const
    -> decltype(ctx.out()) {
    std::string color;
    switch (pt) {
      case universe::point_type::empty:
        color = "\033[31m";
        break; // Red
      case universe::point_type::galaxy:
        color = "\033[32m";
        break; // Green
      default:
        color = "\033[0m";
    }

    return format_to(ctx.out(), "{}{:3}\033[0m", color, static_cast<char>(pt));
  }

} // namespace std
