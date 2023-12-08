#include <algorithm>
#include <aoc/day7.hpp>

#include <cstdint>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <map>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <utility>
#include <vector>

namespace aoc::day7 {
  struct CamelCardHand {
    std::string hand;
    std::uint32_t bid;

    enum class HandType : std::uint8_t {
      Unknown,
      HighCard,
      OnePair,
      TwoPair,
      ThreeOfAKind,
      FullHouse,
      FourOfAKind,
      FiveOfAKind,
    };

    enum class CardType : std::uint8_t {
      NewJoker,
      Two,
      Three,
      Four,
      Five,
      Six,
      Seven,
      Eight,
      Nine,
      Ten,
      Joker,
      Queen,
      King,
      Ace,
    };

    static const std::map<char, CardType> char_to_type;
    static const std::map<HandType, std::string> hand_type_to_string;
    static const std::map<CardType, char> card_type_to_char;

    static const std::vector<
      std::pair<CamelCardHand::HandType,
                const std::function<bool(
                  std::map<CamelCardHand::CardType, std::uint32_t> const&)>>>
      patterns;

    static const std::vector<
      std::pair<CamelCardHand::HandType,
                const std::function<bool(
                  std::map<CamelCardHand::CardType, std::uint32_t> const&)>>>
      patterns_joker;

    friend auto operator>>(std::istream& input, CamelCardHand& handler)
      -> std::istream&;

    friend auto operator<<(std::ostream& output, CamelCardHand const& handler)
      -> std::ostream&;

    friend auto operator<(CamelCardHand const& lhs, CamelCardHand const& rhs)
      -> bool;

    [[nodiscard]] auto evaluate() const -> HandType;
    [[nodiscard]] auto evaluate_joker() const -> HandType;
  };

  auto operator>>(std::istream& input, CamelCardHand::CardType& card_type)
    -> std::istream& {
    char c;
    input >> c;
    card_type = CamelCardHand::char_to_type.at(c);
    return input;
  }

  auto operator<<(std::ostream& output,
                  CamelCardHand::CardType const& card_type) -> std::ostream& {
    output << CamelCardHand::card_type_to_char.at(card_type);
    return output;
  }

  auto operator>>(std::istream& input, CamelCardHand& handler)
    -> std::istream& {
    return input >> handler.hand >> handler.bid;
  }

  auto operator<<(std::ostream& output, CamelCardHand const& handler)
    -> std::ostream& {
    return output << "[hand] " << handler.hand << " " << handler.bid << " => "
                  << CamelCardHand::hand_type_to_string.at(
                       handler.evaluate_joker());
  }

  auto operator<(CamelCardHand const& lhs, CamelCardHand const& rhs) -> bool {
    auto lhs_type = lhs.evaluate();
    auto rhs_type = rhs.evaluate();

    if (lhs_type != rhs_type)
      return lhs_type < rhs_type;

    for (const auto& [lhs_card, rhs_card] :
         std::views::zip(lhs.hand, rhs.hand)) {
      auto lhs_card_type = CamelCardHand::char_to_type.at(lhs_card);
      auto rhs_card_type = CamelCardHand::char_to_type.at(rhs_card);

      if (lhs_card_type != rhs_card_type)
        return lhs_card_type < rhs_card_type;
    }

    return false;
  }

  auto CamelCardHand::evaluate() const -> HandType {
    auto hand_histogram =
      hand | std::views::transform([this](auto const& c) {
        auto card_type = char_to_type.at(c);
        auto count = static_cast<std::uint32_t>(std::ranges::count(hand, c));
        return std::pair { card_type, count };
      }) |
      std::ranges::to<std::map<CardType, std::uint32_t>>();

    auto [hand_type, _] = *std::ranges::find_if(
      patterns,
      [&](auto const& pattern) { return pattern.second(hand_histogram); });

    return hand_type;
  }
  auto CamelCardHand::evaluate_joker() const -> HandType {
    auto hand_histogram =
      hand | std::views::transform([this](auto const& c) {
        auto card_type = char_to_type.at(c);
        auto count = static_cast<std::uint32_t>(std::ranges::count(hand, c));
        return std::pair { card_type, count };
      }) |
      std::ranges::to<std::map<CardType, std::uint32_t>>();
    auto [hand_type, _] = *std::ranges::find_if(
      patterns_joker,
      [&](auto const& pattern) { return pattern.second(hand_histogram); });

    return hand_type;
  }

  auto cmp_joker(CamelCardHand const& lhs, CamelCardHand const& rhs) -> bool {
    auto lhs_type = lhs.evaluate_joker();
    auto rhs_type = rhs.evaluate_joker();

    if (lhs_type != rhs_type)
      return lhs_type < rhs_type;

    for (const auto& [lhs_card, rhs_card] :
         std::views::zip(lhs.hand, rhs.hand)) {
      auto lhs_card_type = CamelCardHand::char_to_type.at(lhs_card);
      auto rhs_card_type = CamelCardHand::char_to_type.at(rhs_card);

      if (lhs_card_type != rhs_card_type) {
        lhs_card_type = lhs_card_type == CamelCardHand::CardType::Joker
          ? CamelCardHand::CardType::NewJoker
          : lhs_card_type;
        rhs_card_type = rhs_card_type == CamelCardHand::CardType::Joker
          ? CamelCardHand::CardType::NewJoker
          : rhs_card_type;
        return lhs_card_type < rhs_card_type;
      }
    }

    return false;
  }

  const std::vector<
    std::pair<CamelCardHand::HandType,
              const std::function<bool(
                std::map<CamelCardHand::CardType, std::uint32_t> const&)>>>
    CamelCardHand::patterns = {
      { CamelCardHand::HandType::FiveOfAKind,
        [](auto const& hand) -> bool {
          return std::ranges::any_of(
            hand, [](auto const& card) -> bool { return card.second == 5; });
        } },
      { CamelCardHand::HandType::FourOfAKind,
        [](auto const& hand) -> bool {
          return std::ranges::any_of(
            hand, [](auto const& card) -> bool { return card.second == 4; });
        } },
      { CamelCardHand::HandType::FullHouse,
        [](auto const& hand) -> bool {
          return std::ranges::any_of(
                   hand,
                   [](auto const& card) -> bool { return card.second == 3; }) &&
            std::ranges::any_of(hand, [](auto const& card) -> bool {
                   return card.second == 2;
                 });
        } },
      { CamelCardHand::HandType::ThreeOfAKind,
        [](auto const& hand) -> bool {
          return std::ranges::any_of(
            hand, [](auto const& card) -> bool { return card.second == 3; });
        } },
      { CamelCardHand::HandType::TwoPair,
        [](auto const& hand) -> bool {
          return std::ranges::count_if(hand, [](auto const& card) -> bool {
                   return card.second == 2;
                 }) == 2;
        } },
      { CamelCardHand::HandType::OnePair,
        [](auto const& hand) -> bool {
          return std::ranges::any_of(
            hand, [](auto const& card) -> bool { return card.second == 2; });
        } },
      { CamelCardHand::HandType::HighCard,
        [](auto const& hand) -> bool {
          return std::ranges::all_of(
            hand, [](auto const& card) -> bool { return card.second == 1; });
        } }
    };

  const std::vector<
    std::pair<CamelCardHand::HandType,
              const std::function<bool(
                std::map<CamelCardHand::CardType, std::uint32_t> const&)>>>
    CamelCardHand::patterns_joker = {
      { CamelCardHand::HandType::FiveOfAKind,
        [](auto const& hand) -> bool {
          auto hand_vec = hand |
            std::ranges::to<std::vector<std::pair<CardType, std::uint32_t>>>();
          std::sort(std::begin(hand_vec), std::end(hand_vec),
                    [](auto const& lhs, auto const& rhs) -> bool {
                      // joker is always higher than any other card
                      if (lhs.first == CardType::Joker) {
                        return true;
                      }
                      if (rhs.first == CardType::Joker) {
                        return false;
                      }
                      return lhs.second > rhs.second;
                    });

          // if the first pair has 5 then we have a 5 of a kind
          if (hand_vec[0].second == 5)
            return true;

          // if the first pair has 4 and the second has 1 Joker then we have a 5
          // of a kind
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 1 &&
              hand_vec[1].second == 4)
            return true;

          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 2 &&
              hand_vec[1].second == 3)
            return true;
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 3 &&
              hand_vec[1].second == 2)
            return true;
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 4 &&
              hand_vec[1].second == 1)
            return true;

          return false;
        } },
      { CamelCardHand::HandType::FourOfAKind,
        [](auto const& hand) -> bool {
          auto hand_vec = hand |
            std::ranges::to<std::vector<std::pair<CardType, std::uint32_t>>>();
          std::sort(std::begin(hand_vec), std::end(hand_vec),
                    [](auto const& lhs, auto const& rhs) -> bool {
                      // joker is always higher than any other card
                      if (lhs.first == CardType::Joker) {
                        return true;
                      }
                      if (rhs.first == CardType::Joker) {
                        return false;
                      }
                      return lhs.second > rhs.second;
                    });

          // if the first pair has 4 then we have a 4 of a kind
          if (hand_vec[0].second == 4)
            return true;

          // if the first pair has 3 and the second has 1 Joker then
          // we have a 4
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 1 &&
              hand_vec[1].second == 3)
            return true;

          // if the first pair has 2 and the second has 2 Joker then
          // we have a 4
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 2 &&
              hand_vec[1].second == 2)
            return true;

          // if the first pair has 1 and the second has 3 Joker then
          // we have a 4
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 3 &&
              hand_vec[1].second == 1)
            return true;

          return false;
        } },
      { CamelCardHand::HandType::FullHouse,
        [](auto const& hand) -> bool {
          auto hand_vec = hand |
            std::ranges::to<std::vector<std::pair<CardType, std::uint32_t>>>();
          std::sort(std::begin(hand_vec), std::end(hand_vec),
                    [](auto const& lhs, auto const& rhs) -> bool {
                      // joker is always higher than any other card
                      if (lhs.first == CardType::Joker) {
                        return true;
                      }
                      if (rhs.first == CardType::Joker) {
                        return false;
                      }
                      return lhs.second > rhs.second;
                    });

          // if the first pair has 3 and the second has 2 then we have a full
          if (hand_vec[0].second == 3 && hand_vec[1].second == 2)
            return true;

          // if the first pair has 3 and the second has 1 Joker then we have a
          // full
          if ((hand_vec[0].first == CardType::Joker &&
               hand_vec[0].second == 1 &&
               (hand_vec[1].second == 2 && hand_vec[2].second == 2)))
            return true;

          return false;
        } },
      { CamelCardHand::HandType::ThreeOfAKind,
        [](auto const& hand) -> bool {
          auto hand_vec = hand |
            std::ranges::to<std::vector<std::pair<CardType, std::uint32_t>>>();
          std::sort(std::begin(hand_vec), std::end(hand_vec),
                    [](auto const& lhs, auto const& rhs) -> bool {
                      // joker is always higher than any other card
                      if (lhs.first == CardType::Joker) {
                        return true;
                      }
                      if (rhs.first == CardType::Joker) {
                        return false;
                      }
                      return lhs.second > rhs.second;
                    });

          if (hand_vec[0].second == 3)
            return true;
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 1 &&
              hand_vec[1].second == 2)
            return true;
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 2 &&
              hand_vec[1].second == 1)
            return true;

          return false;
        } },
      { CamelCardHand::HandType::TwoPair,
        [](auto const& hand) -> bool {
          auto hand_vec = hand |
            std::ranges::to<std::vector<std::pair<CardType, std::uint32_t>>>();
          std::sort(std::begin(hand_vec), std::end(hand_vec),
                    [](auto const& lhs, auto const& rhs) -> bool {
                      if (lhs.first == CardType::Joker) {
                        return true;
                      }
                      if (rhs.first == CardType::Joker) {
                        return false;
                      }
                      return lhs.second > rhs.second;
                    });
          if (hand_vec[0].second == 2 && hand_vec[1].second == 2)
            return true;
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 1 &&
              hand_vec[1].second == 2 && hand_vec[2].second == 2)
            return true;
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 2 &&
              hand_vec[1].second == 1 && hand_vec[2].second == 2)
            return true;

          return false;
        } },
      { CamelCardHand::HandType::OnePair,
        [](auto const& hand) -> bool {
          auto hand_vec = hand |
            std::ranges::to<std::vector<std::pair<CardType, std::uint32_t>>>();
          std::sort(std::begin(hand_vec), std::end(hand_vec),
                    [](auto const& lhs, auto const& rhs) -> bool {
                      if (lhs.first == CardType::Joker) {
                        return true;
                      }
                      if (rhs.first == CardType::Joker) {
                        return false;
                      }
                      return lhs.second > rhs.second;
                    });
          if (hand_vec[0].second == 2)
            return true;
          if (hand_vec[0].first == CardType::Joker && hand_vec[0].second == 1 &&
              hand_vec[1].second == 1)
            return true;
          return false;
        } },
      { CamelCardHand::HandType::HighCard,
        [](auto const& hand) -> bool {
          return std::ranges::all_of(
            hand, [](auto const& card) -> bool { return card.second == 1; });
        } }
    };

  const std::map<char, CamelCardHand::CardType> CamelCardHand::char_to_type = {
    { 'A', CamelCardHand::CardType::Ace },
    { 'K', CamelCardHand::CardType::King },
    { 'Q', CamelCardHand::CardType::Queen },
    { 'J', CamelCardHand::CardType::Joker },
    { 'T', CamelCardHand::CardType::Ten },
    { '9', CamelCardHand::CardType::Nine },
    { '8', CamelCardHand::CardType::Eight },
    { '7', CamelCardHand::CardType::Seven },
    { '6', CamelCardHand::CardType::Six },
    { '5', CamelCardHand::CardType::Five },
    { '4', CamelCardHand::CardType::Four },
    { '3', CamelCardHand::CardType::Three },
    { '2', CamelCardHand::CardType::Two }
  };

  const std::map<CamelCardHand::HandType, std::string>
    CamelCardHand::hand_type_to_string = {
      { CamelCardHand::HandType::HighCard, "High Card" },
      { CamelCardHand::HandType::OnePair, "One Pair" },
      { CamelCardHand::HandType::TwoPair, "Two Pair" },
      { CamelCardHand::HandType::ThreeOfAKind, "Three of a Kind" },
      { CamelCardHand::HandType::FullHouse, "Full House" },
      { CamelCardHand::HandType::FourOfAKind, "Four of a Kind" },
      { CamelCardHand::HandType::FiveOfAKind, "Five of a Kind" }
    };

  const std::map<CamelCardHand::CardType, char>
    CamelCardHand::card_type_to_char = {
      { CamelCardHand::CardType::Ace, 'A' },
      { CamelCardHand::CardType::King, 'K' },
      { CamelCardHand::CardType::Queen, 'Q' },
      { CamelCardHand::CardType::Joker, 'J' },
      { CamelCardHand::CardType::Ten, 'T' },
      { CamelCardHand::CardType::Nine, '9' },
      { CamelCardHand::CardType::Eight, '8' },
      { CamelCardHand::CardType::Seven, '7' },
      { CamelCardHand::CardType::Six, '6' },
      { CamelCardHand::CardType::Five, '5' },
      { CamelCardHand::CardType::Four, '4' },
      { CamelCardHand::CardType::Three, '3' },
      { CamelCardHand::CardType::Two, '2' }
    };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {

    const auto pt1 = [&]() -> std::string {
      auto hands = std::views::istream<CamelCardHand>(input) |
        std::ranges::to<std::vector<CamelCardHand>>();
      std::sort(std::begin(hands), std::end(hands));
      auto indexes = std::views::iota(1U, hands.size() + 1);
      auto hand_index_pairs = std::views::zip(hands, indexes);

      auto sum = std::accumulate(std::begin(hand_index_pairs),
                                 std::end(hand_index_pairs), 0,
                                 [](int acc, auto const& hand_index) {
                                   auto const& [hand, index] = hand_index;
                                   return acc + hand.bid * index;
                                 });

      return std::to_string(sum);
    };

    const auto pt2 = [&]() -> std::string {
      auto hands = std::views::istream<CamelCardHand>(input) |
        std::ranges::to<std::vector<CamelCardHand>>();
      // std::ranges::copy(hands,
      //                  std::ostream_iterator<CamelCardHand>(std::cout,
      //                   "\n"));
      std::sort(std::begin(hands), std::end(hands), cmp_joker);
      auto indexes = std::views::iota(1U, hands.size() + 1);
      auto hand_index_pairs = std::views::zip(hands, indexes);

      auto sum = std::accumulate(std::begin(hand_index_pairs),
                                 std::end(hand_index_pairs), 0,
                                 [](int acc, auto const& hand_index) {
                                   auto const& [hand, index] = hand_index;
                                   return acc + hand.bid * index;
                                 });

      return std::to_string(sum);
    };

    return { pt1, pt2 };
  }
} // namespace aoc::day7
