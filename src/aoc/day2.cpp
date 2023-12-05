#include <algorithm>
#include <aoc/day2.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <istream>
#include <iterator>
#include <numeric>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace aoc::day2 {
  struct Cubes {
    enum class Color {
      red,
      green,
      blue
    };

    friend auto operator>>(std::istream& input, Cubes& cubes) -> std::istream& {
      std::string line;
      std::getline(input, line, ',');

      if (line.empty()) {
        return input;
      }

      std::istringstream stream(line);
      std::string color_str;

      stream >> cubes.count >> std::ws >> color_str;
      cubes.color = str_to_color_map.at(color_str);

      return input;
    }

    friend auto operator<<(std::ostream& output, const Cubes& cubes)
      -> std::ostream& {
      output << "[cubes] count: " << cubes.count
             << ", color: " << color_to_str_map.at(cubes.color);
      return output;
    }

    static const std::unordered_map<std::string, Color> str_to_color_map;
    static const std::unordered_map<Color, std::string> color_to_str_map;
    static const std::unordered_map<Color, size_t> max_permitted_cubes;

    size_t count;
    Color color;
  };

  const std::unordered_map<std::string, Cubes::Color>
    Cubes::str_to_color_map = { { "red", Color::red },
                                { "green", Color::green },
                                { "blue", Color::blue } };
  const std::unordered_map<Cubes::Color, std::string>
    Cubes::color_to_str_map = { { Color::red, "red" },
                                { Color::green, "green" },
                                { Color::blue, "blue" } };

  const std::unordered_map<Cubes::Color, size_t> Cubes::max_permitted_cubes = {
    { Cubes::Color::red, 12 },
    { Cubes::Color::green, 13 },
    { Cubes::Color::blue, 14 }
  };

  struct Set {
    friend auto operator>>(std::istream& input, Set& set) -> std::istream& {
      set.cubes.clear();
      std::string line;
      std::getline(input, line, ';');

      if (line.empty()) {
        return input;
      }

      std::istringstream stream(line);
      for (Cubes cube; stream >> cube;) {
        set.cubes.push_back(std::move(cube));
      }

      return input;
    }

    friend auto operator<<(std::ostream& output, const Set& set)
      -> std::ostream& {
      output << "[set] cubes: " << set.cubes.size() << "\n\t\t";
      std::ranges::copy(set.cubes,
                        std::ostream_iterator<Cubes>(output, "\n\t\t"));
      return output;
    }

    std::vector<Cubes> cubes;
  };

  struct Game {
    friend auto operator>>(std::istream& input, Game& game) -> std::istream& {
      game.sets.clear();
      std::string line;
      std::getline(input, line);

      if (line.empty()) {
        return input;
      }
      std::istringstream stream(line);

      std::string tmp;
      stream >> tmp >> game.id >> tmp;

      for (Set set; stream >> set;) {
        game.sets.push_back(std::move(set));
      }

      return input;
    }

    friend auto operator<<(std::ostream& output, const Game& game)
      -> std::ostream& {
      output << "[game] id: " << game.id << ", sets: " << game.sets.size()
             << "\n\t";
      std::ranges::copy(game.sets, std::ostream_iterator<Set>(output, "\n\t"));
      return output;
    }

    size_t id;
    std::vector<Set> sets;

    [[nodiscard]] auto is_valid() const -> bool {
      return !std::ranges::any_of(sets, [](const Set& set) {
        return std::ranges::any_of(set.cubes, [](const Cubes& cubes) {
          return cubes.count > Cubes::max_permitted_cubes.at(cubes.color);
        });
      });
    };
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>> {

    auto pt1 = [&]() -> std::string {
      return std::to_string(std::accumulate(
        std::istream_iterator<Game>(input), std::istream_iterator<Game>(), 0,
        [&](size_t count, const Game& game) {
          return count + (game.is_valid() ? game.id : 0);
        }));
    };

    return { pt1, [&]() -> std::string {
              return "0";
            } };
  }
} // namespace aoc::day2
