#include <algorithm>
#include <aoc/day15.hpp>

#include <functional>
#include <iostream>
#include <istream>
#include <numeric>
#include <print>
#include <ranges>
#include <utility>

namespace aoc::day15 {
  auto hashmap::insert(sequence_label const& label) -> void {
    auto hashed_seq = std::hash<sequence> {}(label.seq);
    if (std::holds_alternative<sequence_label::operation>(label.op) &&
        map.contains(hashed_seq)) {
      auto& list = map[hashed_seq];
      auto it = std::ranges::find_if(
        list, [&](auto const& pair) { return pair.first == label.seq; });
      if (it != std::end(list)) {
        list.remove(*it);
      }

    } else if (std::holds_alternative<
                 std::pair<sequence_label::operation, std::uint64_t>>(
                 label.op)) {
      auto [_, value] =
        std::get<std::pair<sequence_label::operation, std::uint64_t>>(label.op);
      auto& list = map[hashed_seq];
      auto it = std::ranges::find_if(
        list, [&](auto const& pair) { return pair.first == label.seq; });
      if (it != std::end(list)) {
        it->second = value;
      } else {
        list.emplace_back(label.seq, value);
      }
    }
  }

  auto hashmap::eval() -> std::uint64_t {
    return std::accumulate(
      std::begin(map), std::end(map), 0, [](auto const& acc, auto const& pair) {
        auto& [box, lenses] = pair;
        return acc +
          std::accumulate(
                 std::begin(lenses), std::end(lenses), 0,
                 [box, i = 0](auto const& acc, auto const& pair) mutable {
                   auto& [_, value] = pair;
                   return acc + (box + 1) * (++i * value);
                 });
      });
  }
  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {

    const auto pt1 = [&]() -> std::string {
      auto hashes = std::views::istream<sequence>(input) |
        std::ranges::views::transform(std::hash<sequence> {}) |
        std::ranges::to<std::vector>();
      auto sum = std::accumulate(std::begin(hashes), std::end(hashes), 0);
      return std::to_string(sum);
    };

    const auto pt2 = [&]() -> std::string {
      auto hashes = std::views::istream<sequence_label>(input) |
        std::ranges::to<std::vector>();
      // std::ranges::copy(
      //   hashes, std::ostream_iterator<sequence_label> { std::cout, "\n" });

      auto m = std::accumulate(std::begin(hashes), std::end(hashes), hashmap {},
                               [](auto&& acc, auto const& label) {
                                 acc.insert(label);
                                 return std::move(acc);
                               });

      return std::to_string(m.eval());
    };

    return { pt1, pt2 };
  }
} // namespace aoc::day15

namespace std {
  auto hash<aoc::day15::sequence>::operator()(
    aoc::day15::sequence const& seq) const noexcept -> size_t {
    return std::accumulate(
      std::begin(seq), std::end(seq), 0,
      [](auto const& acc, auto const& c) { return ((acc + c) * 17) % 256; });
  }

  auto operator>>(istream& is, aoc::day15::sequence& seq) -> istream& {
    std::getline(is, seq, ',');
    seq.erase(std::remove(std::begin(seq), std::end(seq), '\n'), std::end(seq));
    return is;
  }

  auto operator>>(istream& is, aoc::day15::sequence_label& seq) -> istream& {
    std::string line;
    std::getline(is, line, ',');
    line.erase(std::remove(std::begin(line), std::end(line), '\n'),
               std::end(line));

    auto dash = line.find('-');
    if (dash != std::string::npos) {
      seq.seq = line.substr(0, dash);
      seq.op = aoc::day15::sequence_label::operation::dash;
    }

    auto equal = line.find('=');
    if (equal != std::string::npos) {

      seq.seq = line.substr(0, equal);
      seq.op =
        std::make_pair(aoc::day15::sequence_label::operation::equal,
                       std::stoul(line.substr(equal + 1, std::string::npos)));
    }
    return is;
  }

  auto operator<<(ostream& os, aoc::day15::sequence_label const& seq)
    -> ostream& {
    if (std::holds_alternative<aoc::day15::sequence_label::operation>(seq.op)) {
      os << "\033[1;31m" << seq.seq
         << std::get<aoc::day15::sequence_label::operation>(seq.op)
         << "\033[0m";

    } else {
      auto [op, value] = std::get<
        std::pair<aoc::day15::sequence_label::operation, std::uint64_t>>(
        seq.op);
      os << "\033[1;32m" << seq.seq << op << "\033[34m" << value << "\033[0m";
    }
    return os;
  }
} // namespace std
