#pragma once

#include <cstdint>
#include <functional>
#include <istream>
#include <map>
#include <utility>
#include <vector>

namespace aoc::day12 {
  struct condition_record {
    enum class spring_type : char {
      operational = '.',
      damaged = '#',
      unknown = '?'
    };

    std::vector<spring_type> conditions;
    std::vector<std::uint64_t> contiguous_damaged;

    static std::map<
      std::pair<std::vector<spring_type>, std::vector<std::uint64_t>>,
      std::uint64_t>
      cache;

    [[nodiscard]] auto permutations() -> std::uint64_t;
    [[nodiscard]] auto to_unfolded() const -> condition_record;

    friend auto operator>>(std::istream& input, condition_record& record)
      -> std::istream&;
    friend auto operator<<(std::ostream& output, condition_record const& record)
      -> std::ostream&;
  };

  auto operator>>(std::istream& input, condition_record::spring_type& type)
    -> std::istream&;
  auto operator<<(std::ostream& output,
                  condition_record::spring_type const& type) -> std::ostream&;
  auto operator<=>(condition_record::spring_type const& lhs,
                   condition_record::spring_type const& rhs)
    -> std::strong_ordering;

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;
} // namespace aoc::day12
