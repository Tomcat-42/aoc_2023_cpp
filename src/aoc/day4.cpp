#include <algorithm>
#include <aoc/dayn.hpp>

#include <forward_list>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <numeric>
#include <print>
#include <ranges>
#include <set>
#include <sstream>
#include <utility>

namespace aoc::day4 {
  struct Cards {
    int id;
    std::set<int> winning_numbers;
    std::set<int> numbers;

    auto friend operator>>(std::istream& input, Cards& cards) -> std::istream& {
      cards.winning_numbers.clear();
      cards.numbers.clear();

      std::string winning_line, numbers_line, tmp;
      std::getline(input, tmp, ':');
      std::getline(input, winning_line, '|');
      std::getline(input, numbers_line);

      if (winning_line.empty() || numbers_line.empty()) {
        return input;
      }

      std::istringstream winning_stream(winning_line),
        numbers_stream(numbers_line), tmp_stream(tmp);

      tmp_stream >> tmp >> cards.id;

      for (int number; winning_stream >> number;) {
        cards.winning_numbers.insert(number);
      }

      for (int number; numbers_stream >> number;) {
        cards.numbers.insert(number);
      }

      return input;
    }

    auto friend operator<<(std::ostream& output, const Cards& game)
      -> std::ostream& {
      output << "[Cards]"
             << " id: " << game.id << std::endl
             << "\t[winning]: ";
      std::ranges::copy(game.winning_numbers,
                        std::ostream_iterator<int>(output, " "));
      output << std::endl << "\t[numbers]: ";
      std::ranges::copy(game.numbers, std::ostream_iterator<int>(output, " "));

      return output;
    }
    [[nodiscard]] auto number_of_hits() const -> int {
      return std::ranges::count_if(numbers, [this](int number) {
        return winning_numbers.contains(number);
      });
    }
  };
  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>> {

    auto pt1 = [&]() -> std::string {
      auto cards = std::views::istream<Cards>(input);
      auto winning = cards | std::views::transform([](const Cards& cards) {
                       auto hits = cards.number_of_hits();
                       return std::max(std::pow(2, hits - 1), 0.0);
                     }) |
        std::ranges::to<std::vector<int>>();
      auto sum = std::accumulate(std::begin(winning), std::end(winning), 0);
      return std::to_string(sum);
    };

    auto pt2 = [&]() -> std::string {
      auto cards = std::views::istream<Cards>(input) |
        std::ranges::to<std::vector<Cards>>();
      auto cards_copies = cards | std::views::transform([](const Cards& card) {
                            return std::pair(card.id, 1);
                          }) |
        std::ranges::to<std::map<int, int>>();

      for (const auto& card : cards) {
        auto id = card.id;
        auto copies = cards_copies[id];
        auto hits = card.number_of_hits();
        for (auto j = id + 1; j <= id + hits && j <= cards.size(); j++) {
          cards_copies[j] += copies;
        }
      }
      auto sum = std::accumulate(
        std::begin(cards_copies), std::end(cards_copies), 0,
        [](int acc, const auto& card) { return acc + card.second; });
      return std::to_string(sum);
    };
    return { pt1, pt2 };
  }
} // namespace aoc::day4
