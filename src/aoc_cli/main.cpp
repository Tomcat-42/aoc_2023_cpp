#include <aoc_cli/days.hpp>

#include <algorithm>
#include <cxxopts.hpp>
#include <format>
#include <fstream>
#include <iostream>
#include <istream>
#include <memory>
#include <print>
#include <ranges>

auto main(int argc, char* argv[]) -> int {
  cxxopts::Options options("aoc_cli", "Advent of Code 2023 CLI");

  // clang-format off
  options.add_options()
    ("day", "Day to run", cxxopts::value<int>()->default_value("1"))
    ("part", "Part to run", cxxopts::value<int>()->default_value("1"))
    ("help", "std::println help")
    ("input", "Input file", cxxopts::value<std::string>()->default_value(""));
  // clang-format on
  options.parse_positional({ "input" });
  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::println("{}", options.help());
    std::exit(0);
  }

  auto day = result["day"].as<int>();
  auto part = result["part"].as<int>();
  auto input = result.count("input")
    ? std::make_unique<std::ifstream>(result["input"].as<std::string>())
    : std::make_unique<std::istream>(std::cin.rdbuf());

  if (aoc_cli::days.count(day) == 0) {
    std::println("Day {} not found", day);
    std::println("Available days:");
    std::ranges::copy(aoc_cli::days | std::views::keys,
                      std::ostream_iterator<int>(std::cout, "\n"));
    std::exit(1);
  }

  auto [part1, part2] = aoc_cli::days.at(day)(*input);
  auto res = part == 1 ? part1() : part2();
  std::println("{}", res);

  return 0;
}
