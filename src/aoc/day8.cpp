#include <aoc/day8.hpp>

#include <cstdint>
#include <cstdio>
#include <format>
#include <ios>
#include <iostream>
#include <istream>
#include <memory>
#include <numeric>
#include <optional>
#include <print>
#include <ranges>
#include <regex>
#include <sstream>
#include <stack>
#include <utility>

namespace aoc::day8 {
  template <typename T>
  auto graph<T>::from_entries(std::ranges::range auto&& entries) -> graph<T> {
    return std::accumulate(
      std::ranges::begin(entries), std::ranges::end(entries),
      graph<T> { T {}, nullptr, nullptr },
      [](graph<T>&& acc, graph<T>::entry const& entry) -> graph<T> {
        auto const& [name, children] = entry;
        auto& [left, right] = children;

        // seach for the node with the name
        auto g = acc.search(name);

        // if the node exists, we update its children
        if (g) {
          auto left_node = acc.search(left);
          if (left_node) {
            (*g)->root.second.first = *left_node;
          } else {
            (*g)->root.second.first = std::make_shared<graph<T>>(left);
          }

          auto right_node = acc.search(right);
          if (right_node) {
            (*g)->root.second.second = *right_node;
          } else {
            (*g)->root.second.second = std::make_shared<graph<T>>(right);
          }

        } else {
          acc.root.first = name;
          acc.root.second.first = std::make_shared<graph<T>>(left);
          if (left != right) {
            acc.root.second.second = std::make_shared<graph<T>>(right);
          } else {
            acc.root.second.second = acc.root.second.first;
          }
        }

        return std::move(acc);
      });
  }

  template <typename T>
  auto graph<T>::search(T const& t) const
    -> std::optional<std::shared_ptr<graph<T>>> {
    std::unordered_set<const graph<T>*> visited;
    std::stack<std::shared_ptr<graph<T>>> s;
    s.push(std::make_shared<graph<T>>(*this));

    while (!s.empty()) {
      auto const curr = s.top();
      s.pop();

      if (visited.find(curr.get()) != visited.end()) {
        continue;
      }
      visited.insert(curr.get());

      if (curr->root.first == t) {
        return curr;
      }

      if (curr->root.second.first) {
        s.push(curr->root.second.first);
      }

      if (curr->root.second.second) {
        s.push(curr->root.second.second);
      }
    }

    return std::nullopt;
  }

  template <typename T>
  auto graph<T>::traverse(std::ranges::range auto&& instructions, T begin,
                          T end) const -> std::optional<std::uint32_t> {

    // std::unordered_set<const graph<T>*> visited;
    std::stack<std::pair<std::shared_ptr<graph<T>>, std::uint32_t>> s;
    std::uint32_t path_length = 0;

    auto node = search(begin);
    if (!node) {
      return std::nullopt;
    }

    s.push({ *node, path_length });
    while (!s.empty()) {
      auto const [curr, curr_path_length] = s.top();
      s.pop();

      // if (visited.find(curr.get()) != visited.end()) {
      //   continue;
      // }
      // visited.insert(curr.get());

      if (curr->root.first == end) {
        return curr_path_length;
      }
      switch (instructions[curr_path_length % instructions.size()]) {
      case instruction::left:
        if (curr->root.second.first) {
          s.push({ curr->root.second.first, curr_path_length + 1 });
        }
        break;
      case instruction::right:
        if (curr->root.second.second) {
          s.push({ curr->root.second.second, curr_path_length + 1 });
        }
        break;
      }
    }

    return std::nullopt;
  }

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {
    const auto instructions = std::ranges::views::istream<instruction>(input) |
      std::ranges::to<std::vector<instruction>>();
    const auto entries =
      std::ranges::views::istream<graph<std::string>::entry>(input) |
      std::ranges::to<std::vector<graph<std::string>::entry>>();
    const auto g = graph<std::string>::from_entries(entries);
    std::println("{}", g);

    const auto pt1 = [=]() -> std::string {
      return std::to_string(g.traverse(instructions, "AAA", "ZZZ").value_or(0));
    };

    const auto pt2 = []() -> std::string {
      return "not implemented yet";
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
  auto operator>>(std::istream& is, aoc::day8::graph<std::string>::entry& e)
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

  auto operator<<(std::ostream& os, aoc::day8::graph<std::string>::entry e)
    -> std::ostream& {
    return os << e.first << " = (" << e.second.first << ", " << e.second.second
              << ")";
  }

} // namespace std
