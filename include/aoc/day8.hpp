#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <istream>
#include <ranges>
#include <utility>
#include <unordered_set>

namespace aoc::day8 {
  enum class instruction : char {
    left = 'L',
    right = 'R',
  };

  auto operator>>(std::istream& is, aoc::day8::instruction& i) -> std::istream&;
  auto operator<<(std::ostream& os, aoc::day8::instruction i) -> std::ostream&;

  template <typename T>
  struct graph {
    std::pair<T,
              std::pair<std::shared_ptr<graph<T>>, std::shared_ptr<graph<T>>>>
      root;

    graph(T const& value, std::shared_ptr<graph<T>> left = nullptr,
          std::shared_ptr<graph<T>> right = nullptr)
      : root { value, { std::move(left), std::move(right) } } {}
    using entry = std::pair<T, std::pair<T, T>>;

    static auto from_entries(std::ranges::range auto&& entries) -> graph;
    [[nodiscard]] auto search(T const& t) const
      -> std::optional<std::shared_ptr<graph<T>>>;
    [[nodiscard]] auto traverse(std::ranges::range auto&& instructions, T begin,
                                T end) const -> std::optional<std::uint32_t>;
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>,
                 std::function<std::string()>> const;

} // namespace aoc::day8

namespace std {
  auto operator>>(std::istream& is, aoc::day8::graph<std::string>::entry& e)
    -> std::istream&;
  auto operator<<(std::ostream& os, aoc::day8::graph<std::string>::entry e)
    -> std::ostream&;

  template <typename T>
  using dag = aoc::day8::graph<T>;

template <typename T>
struct std::formatter<dag<T>> {
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const dag<T>& g, FormatContext& ctx) const {
        std::unordered_set<const dag<T>*> visited;
        std::function<std::string(const dag<T>&, const std::string&)> walk =
            [&walk, &visited](const dag<T>& graph, const std::string& prefix) -> std::string {
            if (visited.find(&graph) != visited.end()) {
                // Node already visited, return to avoid infinite loop
                return std::format("{}[...]\n", prefix);
            }
            visited.insert(&graph); // Mark this node as visited

            std::string s;
            auto left = graph.root.second.first;
            auto right = graph.root.second.second;

            if (left) {
                auto name = left->root.first;
                s += std::format("{}├──\033[33m{}\033[0m<\033[34m{:p}\033[0m>\n",
                                 prefix, name, static_cast<void*>(left.get()));
                s += walk(*left, std::format("{}│  ", prefix));
            }
            if (right) {
                s += std::format("{}└──\033[33m{}\033[0m<\033[34m{:p}\033[0m>\n",
                                 prefix, right->root.first,
                                 static_cast<const void*>(right.get()));
                s += walk(*right, std::format("{}   ", prefix));
            }

            return s;
        };
        return std::format_to(
            ctx.out(), "\033[32m{}\033[0m<\033[34m{:p}\033[0m>\n{}", g.root.first,
            static_cast<const void*>(&g), walk(g, ""));
    }
};

} // namespace std
