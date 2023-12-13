#include <aoc/day12.hpp>

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <regex>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace aoc::day12 {
  auto condition_record::permutations() -> std::uint64_t {
    std::map<std::pair<std::vector<spring_type>, std::vector<std::uint64_t>>,
             std::uint64_t>
      cache;

    std::function<std::uint64_t(decltype(conditions),
                                decltype(contiguous_damaged))>
      solve =
        [&solve, &cache](decltype(conditions) cfg,
                         decltype(contiguous_damaged) nums) -> std::uint64_t {
      if (cfg.empty())
        return nums.empty() ? 1 : 0;

      if (nums.empty())
        return std::ranges::find(cfg, spring_type::damaged) == std::end(cfg)
          ? 1
          : 0;

      if (cache.contains({ cfg, nums }))
        return cache[{ cfg, nums }];

      std::uint64_t result = 0;

      if (cfg[0] == spring_type::operational ||
          cfg[0] == spring_type::unknown) {
        result += solve(
          std::vector<spring_type>(std::next(std::begin(cfg)), std::end(cfg)),
          nums);
      }

      if (cfg[0] == spring_type::damaged || cfg[0] == spring_type::unknown) {
        auto cfg_len = cfg.size();
        if (nums[0] <= cfg_len &&
            std::none_of(
              std::begin(cfg), std::next(std::begin(cfg), nums[0]),
              [](spring_type s) { return s == spring_type::operational; }) &&
            (nums[0] == cfg_len || cfg[nums[0]] != spring_type::damaged)) {

          result +=
            solve(std::vector<spring_type>(
                    std::next(std::begin(cfg), std::min(nums[0] + 1, cfg_len)),
                    std::end(cfg)),
                  std::vector<std::uint64_t>(std::next(std::begin(nums)),
                                             std::end(nums)));
        }
      }

      cache[{ cfg, nums }] = result;
      return result;
    };

    return solve(conditions, contiguous_damaged);
  }

  auto condition_record::to_unfolded() const -> condition_record {
    auto new_conditions = this->conditions;
    auto new_contiguous_damaged = this->contiguous_damaged;

    for (auto i = 0; i < 4; ++i) {
      new_conditions.push_back(spring_type::unknown);
      new_conditions.insert(std::end(new_conditions), std::begin(conditions),
                            std::end(conditions));
      new_contiguous_damaged.insert(std::end(new_contiguous_damaged),
                                    std::begin(contiguous_damaged),
                                    std::end(contiguous_damaged));
    }
    return { new_conditions, new_contiguous_damaged };
  }

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const {
    auto conditions = std::views::istream<condition_record>(input) |
      std::ranges::to<std::vector<condition_record>>();

    const auto pt1 = [=]() mutable -> std::string {
      auto sum = std::accumulate(std::begin(conditions), std::end(conditions),
                                 0, [](auto const& acc, auto& record) {
                                   return acc + record.permutations();
                                 });
      return std::to_string(sum);
    };

    const auto pt2 = [=]() -> std::string {
      auto sum = std::transform_reduce(
        std::begin(conditions), std::end(conditions), 0ull, std::plus<> {},
        [](auto const& record) { return record.to_unfolded().permutations(); });
      return std::to_string(sum);
    };

    return { pt1, pt2 };
  }

  // boring parsing stuff
  auto operator>>(std::istream& input, condition_record& record)
    -> std::istream& {
    record.conditions.clear();
    record.contiguous_damaged.clear();

    std::string line;
    input >> line;
    auto stream = std::stringstream { line };
    std::ranges::copy(
      std::views::istream<condition_record::spring_type>(stream),
      std::back_inserter(record.conditions));

    input >> line;
    auto iss = std::istringstream { std::regex_replace(
      line, std::regex { R"(,)" }, " ") };
    std::ranges::copy(std::views::istream<std::uint64_t>(iss),
                      std::back_inserter(record.contiguous_damaged));

    return input;
  }

  auto operator<<(std::ostream& output, condition_record const& record)
    -> std::ostream& {
    output << "\033[1;34m[conditions]\033[0m "; // Bright Blue
    std::ranges::copy(
      record.conditions,
      std::ostream_iterator<condition_record::spring_type>(output, " "));
    output << "\033[1;35m => \033[0m "; // Bright Magenta
    output << "\033[1;36m";             // Bright Cyan
    std::ranges::copy(record.contiguous_damaged,
                      std::ostream_iterator<std::uint64_t>(output, " "));
    output << "\033[0m";

    return output;
  }

  auto operator>>(std::istream& input, condition_record::spring_type& type)
    -> std::istream& {
    char c;
    input >> c;
    type = static_cast<condition_record::spring_type>(c);
    return input;
  }
  auto operator<<(std::ostream& output,
                  condition_record::spring_type const& type) -> std::ostream& {
    switch (type) {
      case condition_record::spring_type::operational:
        output << "\033[1;32m"; // Bright Green
        break;
      case condition_record::spring_type::damaged:
        output << "\033[1;31m"; // Bright Red
        break;
      case condition_record::spring_type::unknown:
        output << "\033[1;33m"; // Bright Yellow
        break;
      default:
        break;
    }

    output << static_cast<char>(type);
    return output << "\033[0m";
  }
  auto operator<=>(condition_record::spring_type const& lhs,
                   condition_record::spring_type const& rhs)
    -> std::strong_ordering {
    return static_cast<char>(lhs) <=> static_cast<char>(rhs);
  }

} // namespace aoc::day12
