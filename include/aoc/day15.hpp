#pragma once

#include <cstdint>
#include <functional>
#include <istream>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <variant>

namespace aoc::day15 {
  using sequence = std::string;

  struct sequence_label {
    enum operation : char {
      dash = '-',
      equal = '=',
    };
    sequence seq;
    std::variant<operation, std::pair<operation, std::uint64_t>> op;
  };

  struct hashmap {
    std::map<std::uint64_t, std::list<std::pair<sequence, std::uint64_t>>> map;
    auto insert(sequence_label const& label) -> void;
    auto eval() -> std::uint64_t;
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;
} // namespace aoc::day15

namespace std {
  template <>
  struct hash<aoc::day15::sequence> {
    auto operator()(aoc::day15::sequence const& seq) const noexcept -> size_t;
  };

  auto operator>>(istream& is, aoc::day15::sequence& seq) -> istream&;
  auto operator>>(istream& is, aoc::day15::sequence_label& seq) -> istream&;
  auto operator<<(ostream& os, aoc::day15::sequence_label const& seq)
    -> ostream&;
} // namespace std
