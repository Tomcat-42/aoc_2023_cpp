#include <aoc/dayn.hpp>

#include <cstdint>
#include <functional>
#include <iostream>
#include <istream>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace aoc::day3 {

  /* interval = <i, <j1, j2>>
   * where:
   *  i is row number
   *  j1 is the start of the column range
   *  j2 is the end of the column range
   *
   * this means that the interval is all the points from (i, j1) to (i, j2)
   * inclusive are in the interval
   */
  using interval = std::pair<size_t, std::pair<size_t, size_t>>;

  /* point = <i, j>
   * where:
   *  i is row number
   *  j is column number
   *  this is a point in the grid
   */
  using point = std::pair<size_t, size_t>;

  /* number_part = <number, interval>
   * where:
   *  number is the number in the grid
   *  interval is the interval that the number occupies
   */
  using number_part = std::pair<std::uint32_t, interval>;

  /* symbol = <char, point>
   * where:
   *  char is the symbol in the grid
   *  point is the point in the grid that the symbol is at
   */
  using symbol = std::pair<char, point>;

  auto parse_engine(std::istream& input)
    -> std::pair<std::vector<number_part>, std::vector<symbol>> {

    std::vector<number_part> number_parts;
    std::vector<symbol> symbols;

    auto lines = std::ranges::istream_view<std::string>(input) |
      std::ranges::to<std::vector<std::string>>();

    for (size_t i = 0; i < lines.size(); i++) {
      auto line = lines[i];
      for (size_t j = 0; j < line.size(); j++) {
        auto c = line[j];
        /* if c is a '.' => skip
         * if c is a digit => parse the number (all digits until a non-digit)
         * otherwise => add to symbols
         */
        if (c == '.') {
          continue;
        } else if (std::isdigit(c)) {
          /* parse the number */
          size_t start = j;
          size_t end = j;
          while (end < line.size() && std::isdigit(line[end])) {
            end++;
          }
          j = end - 1;
          auto number = std::stoull(line.substr(start, end - start));
          number_parts.push_back({ number, { i, { start, end - 1 } } });
        } else {
          symbols.push_back({ c, { i, j } });
        }
      }
    }
    return { number_parts, symbols };
  }

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>> {
    auto [number_parts, symbols] = parse_engine(input);

    std::map<point, number_part> number_parts_map;
    std::map<symbol, std::set<number_part>> symbol_number_parts_map;

    /* for each number, add it to the grid
     * where the grid is a map from point to interval
     */
    for (auto& part : number_parts) {
      auto& [number, interval] = part;
      auto& [i, range] = interval;
      auto& [j1, j2] = range;
      for (size_t j = j1; j <= j2; j++) {
        number_parts_map[{ i, j }] = part;
      }
    }

    /* for each symbol, find its 8-neighbors
     * then, for each neighbor, get the number part
     * then, add the number part to the symbol's set
     */
    for (auto& symbol : symbols) {
      auto& [c, pos] = symbol;
      auto& [i, j] = pos;
      // there will be invalid points, but that's ok
      std::vector<point> neighbors = { { i - 1, j - 1 }, { i - 1, j },
                                       { i - 1, j + 1 }, { i, j - 1 },
                                       { i, j + 1 },     { i + 1, j - 1 },
                                       { i + 1, j },     { i + 1, j + 1 } };
      auto unique_entries = neighbors |
        std::ranges::views::filter([&](auto& p) {
                              return number_parts_map.contains(p);
                            }) |
        std::views::transform([&](auto& p) { return number_parts_map.at(p); }) |
        std::ranges::to<std::set<std::pair<std::uint32_t, interval>>>();
      symbol_number_parts_map[symbol] = unique_entries;
    }

    auto pt1 = [=]() {
      /* sum all the numbers_part.first in the set of each symbol entry in the
       * symbol_number_parts_map*/
      auto sum = std::accumulate(
        symbol_number_parts_map.begin(), symbol_number_parts_map.end(), 0ull,
        [](auto acc, auto& entry) {
          auto& [symbol, number_parts] = entry;
          return acc +
            std::accumulate(number_parts.begin(), number_parts.end(), 0ull,
                            [](auto acc, auto& number_part) {
                              auto& [number, interval] = number_part;
                              return acc + number;
                            });
        });
      return std::to_string(sum);
    };

    auto pt2 = [=]() {
      // a gear is all '*'s witch has 2 neighbors
      auto gears =
        symbol_number_parts_map | std::ranges::views::filter([&](auto& entry) {
          auto& [symbol, number_parts] = entry;
          return symbol.first == '*' && number_parts.size() == 2;
        }) |
        std::views::transform([&](auto& entry) {
          auto& [symbol, number_parts] = entry;
          return std::accumulate(number_parts.begin(), number_parts.end(), 1ull,
                                 [](auto acc, auto& number_part) {
                                   auto& [number, interval] = number_part;
                                   return acc * number;
                                 });
        }) |
        std::ranges::to<std::vector<std::uint32_t>>();
      // sum all the gears
      auto sum = std::accumulate(gears.begin(), gears.end(), 0ull);
      return std::to_string(sum);
    };
    return { pt1, pt2 };
  }
} // namespace aoc::day3
