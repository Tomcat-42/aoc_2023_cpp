#pragma once

#include <functional>
#include <istream>
#include <optional>
#include <utility>
#include <vector>

namespace aoc::day13 {
  struct pattern {
    std::vector<std::string> lines;
    enum class reflection_type {
      horizontal,
      vertical
    };

    [[nodiscard]] auto find_reflections() const -> std::optional<std::vector<
      std::pair<std::uint64_t, std::pair<std::uint64_t, reflection_type>>>>;

    [[nodiscard]] auto flip_all_and_find_different_reflection()
      -> std::uint64_t;
    friend auto operator>>(std::istream& input, pattern& p) -> std::istream&;
    friend auto operator<<(std::ostream& output, pattern const& p)
      -> std::ostream&;
  };
  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;
} // namespace aoc::day13
