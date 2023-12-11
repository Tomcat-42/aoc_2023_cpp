#include <algorithm>
#include <aoc/day10.hpp>

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <istream>
#include <map>
#include <print>
#include <queue>
#include <set>
#include <utility>

namespace aoc::day10 {
  auto maze::parse(std::istream& input) -> const maze {
    std::vector<std::string> lines;
    for (std::string line; std::getline(input, line);)
      lines.push_back(line);

    std::pair<size_t, size_t> start;
    std::vector<std::vector<tile_type>> matrix;
    for (size_t i = 0; i < lines.size(); i++) {
      std::vector<tile_type> row;
      for (size_t j = 0; j < lines[i].size(); j++) {
        const auto c = lines[i][j];
        if (c == 'S')
          start = { i, j };

        row.push_back(tile_map.at(c));
      }
      matrix.push_back(row);
    }
    auto m = maze { matrix, start };
    // replace start with its actual type
    auto neighbors = std::array { position { start.first - 1, start.second },
                                  position { start.first, start.second + 1 },
                                  position { start.first + 1, start.second },
                                  position { start.first, start.second - 1 } };
    auto max_row = matrix.size();
    auto max_col = matrix[0].size();
    auto neighbors_type = std::array<tile_type, 4> {
      (neighbors[0].first >= 0 && neighbors[0].first < max_row &&
       neighbors[0].second >= 0 && neighbors[0].second < max_col)
        ? matrix[neighbors[0].first][neighbors[0].second]
        : tile_type::empty,

      (neighbors[1].first >= 0 && neighbors[1].first < max_row &&
       neighbors[1].second >= 0 && neighbors[1].second < max_col)
        ? matrix[neighbors[1].first][neighbors[1].second]
        : tile_type::empty,

      (neighbors[2].first >= 0 && neighbors[2].first < max_row &&
       neighbors[2].second >= 0 && neighbors[2].second < max_col)
        ? matrix[neighbors[2].first][neighbors[2].second]
        : tile_type::empty,

      (neighbors[3].first >= 0 && neighbors[3].first < max_row &&
       neighbors[3].second >= 0 && neighbors[3].second < max_col)
        ? matrix[neighbors[3].first][neighbors[3].second]
        : tile_type::empty
    };
    m.matrix[start.first][start.second] = m.deduce_type(neighbors_type);

    return m;
  }

  auto maze::deduce_type(std::array<tile_type, 4> clockwise_four_neighbors)
    -> const tile_type {
    auto could_connect_in_right =
      std::set { tile_type::left_right, tile_type::up_left,
                 tile_type::left_down };
    auto could_connect_in_left =
      std::set { tile_type::left_right, tile_type::up_right,
                 tile_type::right_down };
    auto could_connect_in_up =
      std::set { tile_type::up_down, tile_type::left_down,
                 tile_type::right_down };
    auto could_connect_in_down =
      std::set { tile_type::up_down, tile_type::up_left, tile_type::up_right };

    // case 1: up_down
    if (could_connect_in_up.contains(clockwise_four_neighbors[0]) &&
        could_connect_in_down.contains(clockwise_four_neighbors[2]))
      return tile_type::up_down;
    // case 2: left_right
    if (could_connect_in_left.contains(clockwise_four_neighbors[3]) &&
        could_connect_in_right.contains(clockwise_four_neighbors[1]))
      return tile_type::left_right;
    // case 3: up_right
    if (could_connect_in_up.contains(clockwise_four_neighbors[0]) &&
        could_connect_in_right.contains(clockwise_four_neighbors[1]))
      return tile_type::up_right;
    // case 4: up_left
    if (could_connect_in_up.contains(clockwise_four_neighbors[0]) &&
        could_connect_in_left.contains(clockwise_four_neighbors[3]))
      return tile_type::up_left;
    // case 5: left_down
    if (could_connect_in_left.contains(clockwise_four_neighbors[3]) &&
        could_connect_in_down.contains(clockwise_four_neighbors[2]))
      return tile_type::left_down;
    // case 6: right_down
    if (could_connect_in_right.contains(clockwise_four_neighbors[1]) &&
        could_connect_in_down.contains(clockwise_four_neighbors[2]))
      return tile_type::right_down;

    return tile_type::empty;
  }

  auto maze::connected_vertices(position p) const -> std::vector<position> {
    std::vector<position> neighbors;

    auto could_connect_in_right =
      std::set { tile_type::left_right, tile_type::up_left,
                 tile_type::left_down };
    auto could_connect_in_left =
      std::set { tile_type::left_right, tile_type::up_right,
                 tile_type::right_down };
    auto could_connect_in_up =
      std::set { tile_type::up_down, tile_type::left_down,
                 tile_type::right_down };
    auto could_connect_in_down =
      std::set { tile_type::up_down, tile_type::up_left, tile_type::up_right };

    auto curr = matrix[p.first][p.second];

    /* check up. allowed types: up_down, left_down, right_down
     start*/
    if (p.first > 0) {
      const auto up = matrix[p.first - 1][p.second];
      if (could_connect_in_down.contains(curr) &&
          could_connect_in_up.contains(up))
        neighbors.emplace_back(p.first - 1, p.second);
    }

    /* check right. allowed types: left_right, up_left, left_down and
     * start*/
    if (p.second < matrix[p.first].size() - 1) {
      const auto right = matrix[p.first][p.second + 1];
      if (could_connect_in_left.contains(curr) &&
          could_connect_in_right.contains(right))
        neighbors.emplace_back(p.first, p.second + 1);
    }

    /* check down. allowed types: up_down, up_right, up_left and
     start*/
    if (p.first < matrix.size() - 1) {
      const auto down = matrix[p.first + 1][p.second];
      if (could_connect_in_up.contains(curr) &&
          could_connect_in_down.contains(down))
        neighbors.emplace_back(p.first + 1, p.second);
    }

    /* check left. allowed types: left_right, up_right, right_down and
     start*/
    if (p.second > 0) {
      const auto left = matrix[p.first][p.second - 1];
      if (could_connect_in_right.contains(curr) &&
          could_connect_in_left.contains(left))
        neighbors.emplace_back(p.first, p.second - 1);
    }

    return neighbors;
  }

  auto maze::depth() const -> std::uint64_t {
    std::set<position> visited;
    std::queue<std::pair<position, std::uint64_t>> q;

    std::uint64_t depth = 0;
    q.emplace(start, 0);
    while (!q.empty()) {
      auto [p, level] = q.front();
      q.pop();

      if (visited.contains(p))
        continue;

      visited.insert(p);
      depth = std::max(depth, level);
      for (const auto& n : connected_vertices(p))
        q.emplace(n, level + 1);
    }
    return depth;
  }

  auto maze::loop() const -> std::set<position> {
    std::set<position> visited;
    std::stack<position> q;

    q.emplace(start);
    while (!q.empty()) {
      auto p = q.top();
      q.pop();

      if (visited.contains(p))
        continue;

      visited.insert(p);
      for (const auto& n : connected_vertices(p))
        q.emplace(n);
    }
    return visited;
  }

  auto maze::connected_area() const -> std::uint64_t {
    auto area_scanline = [this](std::set<position> path) {
      auto points = 0;
      for (size_t i = 0; i < matrix.size(); i++) {
        auto inside = false;
        for (size_t j = 0; j < matrix[i].size(); j++) {
          auto tile = matrix[i][j];
          switch (tile) {
            case tile_type::up_down:
            case tile_type::right_down:
            case tile_type::left_down:
              if (path.contains({ i, j }))
                inside = !inside;
            default:
              break;
          }

          if (inside && !path.contains({ i, j }))
            points++;
        }
      }

      return points;
    };

    return area_scanline(loop());
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {
    const auto m = maze::parse(input);

    const auto pt1 = [=]() -> std::string {
      return std::to_string(m.depth());
    };

    const auto pt2 = [=]() -> std::string {
      std::println("{}", m);
      // std::println("{}", m.connected_vertices({ 3, 2 }));
      // auto loop = m.loop();
      return std::to_string(m.connected_area());
      return "not implemented";
    };

    return { pt1, pt2 };
  }

  const std::map<char, maze::tile_type> maze::tile_map = {
    { '|', tile_type::up_down },   { '-', tile_type::left_right },
    { 'L', tile_type::up_right },  { 'J', tile_type::up_left },
    { '7', tile_type::left_down }, { 'F', tile_type::right_down },
    { '.', tile_type::empty },     { 'S', tile_type::start }
  };

  const std::map<maze::tile_type, char> maze::char_map = {
    { tile_type::up_down, '|' },   { tile_type::left_right, '-' },
    { tile_type::up_right, 'L' },  { tile_type::up_left, 'J' },
    { tile_type::left_down, '7' }, { tile_type::right_down, 'F' },
    { tile_type::empty, '.' },     { tile_type::start, 'S' },
    { tile_type::debug, 'O' }

  };
} // namespace aoc::day10
