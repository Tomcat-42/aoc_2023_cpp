#include <algorithm>
#include <aoc/day8.hpp>

#include <cstdint>
#include <iostream>
#include <istream>
#include <numeric>
#include <print>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

namespace aoc::day8 {
  auto graph::from_entries(std::ranges::range auto&& entries) -> graph {
    auto nodes_map = std::accumulate(
      std::ranges::begin(entries), std::ranges::end(entries),
      std::map<std::string, std::pair<std::string, std::string>> {},
      [](auto&& acc, graph::entry const& entry) {
        auto const& [name, children] = entry;
        acc[name] = children;
        return std::move(acc);
      });

    return { .nodes = std::move(nodes_map) };
  }

  auto graph::traverse(std::ranges::range auto&& instructions, std::regex begin,
                       std::regex end) const -> std::uint64_t {
    auto steps_from_to = [&](auto from, auto to) {
      std::uint64_t steps = 0;
      while (!std::regex_match(from, to)) {
        switch (instructions[steps++ % instructions.size()]) {
        case instruction::left:
          from = std::get<0>(nodes.at(from));
          break;
        case instruction::right:
          from = std::get<1>(nodes.at(from));
          break;
        }
      }

      return steps;
    };

    auto steps = nodes | std::views::keys | std::views::filter([&](auto node) {
                   return std::regex_match(node, begin);
                 }) |
      std::views::transform(
                   [&](auto node) { return steps_from_to(node, end); }) |
      std::ranges::to<std::vector<int>>();

    return std::accumulate(
      std::ranges::begin(steps), std::ranges::end(steps), 1ull,
      [](auto m, auto n) { return m / std::gcd(m, n) * n; });
  }

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {
    const auto instructions = std::ranges::views::istream<instruction>(input) |
      std::ranges::to<std::vector<instruction>>();
    const auto entries = std::ranges::views::istream<graph::entry>(input) |
      std::ranges::to<std::vector<graph::entry>>();
    const auto g = graph::from_entries(entries);

    const auto pt1 = [=]() -> std::string {
      return std::to_string(
        g.traverse(instructions, std::regex { "AAA" }, std::regex { "ZZZ" }));
    };

    const auto pt2 = [=]() -> std::string {
      return std::to_string(
        g.traverse(instructions, std::regex { ".*A$" }, std::regex { ".*Z$" }));

      return "not implemented";
    };

    return { pt1, pt2 };
  }

  // boring parsing stuff
  auto operator>>(std::istream& is, instruction& i) -> std::istream& {
    char c;
    is >> std::noskipws >> c;
    switch (c) {
    case 'L':
      i = instruction::left;
      return is;
    case 'R':
      i = instruction::right;
      return is;
    default:
      is.setstate(std::ios::failbit);
      return is;
    }
  }

  auto operator<<(std::ostream& os, instruction i) -> std::ostream& {
    switch (i) {
    case instruction::left:
      return os << "L";
    case instruction::right:
      return os << "R";
    }
  }

} // namespace aoc::day8

namespace std {
  auto operator>>(std::istream& is, aoc::day8::graph::entry& e)
    -> std::istream& {
    std::string line, name, left, right;

    if (is.fail()) {
      is.clear();
    }

    // skip empty lines
    while (is.peek() == '\n') {
      is.ignore();
    }
    std::getline(is, line);

    std::istringstream iss { std::regex_replace(
      line, std::regex { R"((=|,)|\(|\))" }, " ") };
    iss >> name >> left >> right;
    e = { name, { left, right } };
    return is;
  }

  auto operator<<(std::ostream& os, aoc::day8::graph::entry e)
    -> std::ostream& {
    return os << e.first << " = (" << e.second.first << ", " << e.second.second
              << ")";
  }

} // namespace std
