#include <aoc/day5.hpp>

#include <algorithm>
#include <cstdint>
#include <functional>
#include <ios>
#include <iostream>
#include <istream>
#include <limits>
#include <map>
#include <print>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

namespace aoc::day5 {
  using range = std::pair<std::uint32_t, std::uint32_t>;
  using entry = std::pair<std::uint32_t, range>;

  class interval_map {
    std::map<std::uint32_t, range> map;
    interval_map(std::map<std::uint32_t, range> init_map)
      : map(std::move(init_map)) {}

  public:
    static auto from_entries(std::vector<entry>&& entries) -> interval_map;
    [[nodiscard]] auto search(std::uint32_t key) const -> std::uint32_t;
  };

  class almanac {
    almanac(std::vector<std::uint32_t> seeds, std::vector<interval_map> maps)
      : seeds(std::move(seeds)), maps(std::move(maps)) {}

  public:
    std::vector<std::uint32_t> seeds;
    std::vector<interval_map> maps;

    auto static from_str(const std::string&& str) -> almanac;

    [[nodiscard]] auto map_seeds(const std::vector<std::uint32_t>& seeds) const
      -> std::uint32_t {
      const size_t num_threads =
        std::max(1u, std::thread::hardware_concurrency());
      const size_t chunk_size = seeds.size() / num_threads;
      std::vector<std::thread> threads;
      std::vector<std::uint32_t> min_results(
        num_threads, std::numeric_limits<std::uint32_t>::max());

      for (size_t i = 0; i < num_threads; ++i) {
        size_t start_index = i * chunk_size;
        size_t end_index =
          (i == num_threads - 1) ? seeds.size() : (i + 1) * chunk_size;

        threads.emplace_back(
          [this, &seeds, start_index, end_index, &min_results, i]() {
            for (size_t j = start_index; j < end_index; ++j) {
              std::uint32_t result = seeds[j];
              for (const auto& map : maps) {
                result = map.search(result);
              }
              min_results[i] = std::min(min_results[i], result);
            }
          });
      }

      for (auto& thread : threads) {
        if (thread.joinable()) {
          thread.join();
        }
      }

      return *std::min_element(min_results.begin(), min_results.end());
    }
  };

  auto solution(std::istream& input)
    -> std::pair<std::function<std::string()>, std::function<std::string()>> {
    const auto str = std::string { std::istreambuf_iterator<char> { input },
                                   std::istreambuf_iterator<char> {} };
    const auto almanac = almanac::from_str(std::move(str));

    const auto pt1 = [=]() {
      return std::to_string(almanac.map_seeds(almanac.seeds));
    };

    const auto pt2 = [=]() {
      auto pairs = std::views::iota(0u, almanac.seeds.size() / 2) |
        std::views::transform([=](auto i) {
                     auto first = almanac.seeds[i * 2];
                     auto count = almanac.seeds[i * 2 + 1];
                     return std::views::iota(first, first + count);
                   }) |
        std::views::join | std::views::common | std::ranges::to<std::vector>();

      return std::to_string(almanac.map_seeds(pairs));
    };

    return { pt1, pt2 };
  }

  auto operator>>(std::istream& input, entry& entry) -> std::istream& {
    input >> entry.second.first;
    input >> entry.first;
    input >> entry.second.second;

    return input;
  }

  // almanac implementation
  auto almanac::from_str(const std::string&& str) -> almanac {

    std::stringstream stream { str };
    std::string chunk;

    std::vector<std::uint32_t> seeds;

    stream.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    std::getline(stream, chunk);
    std::stringstream line_stream { chunk };
    std::ranges::copy(std::ranges::istream_view<std::uint32_t>(line_stream),
                      std::back_inserter(seeds));

    // parsing maps (each map delimiter is a blank line)
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    chunk = std::string { std::istreambuf_iterator<char> { stream },
                          std::istreambuf_iterator<char> {} };
    chunk = std::regex_replace(chunk, std::regex { "\n\n" }, "*");

    auto maps =
      std::views::split(chunk, '*') | std::views::transform([](auto&& range) {
        auto str = std::string { std::begin(range), std::end(range) };
        auto stream = std::stringstream { str };
        // ignore first line
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::vector<entry> entries;
        for (entry e; stream >> e;) {
          entries.push_back(e);
        }
        return interval_map::from_entries(std::move(entries));
      }) |
      std::ranges::to<std::vector<interval_map>>();

    return { seeds, maps };
  }

  // interval_map implementation
  auto interval_map::from_entries(std::vector<entry>&& entries)
    -> interval_map {
    std::map<std::uint32_t, range> temp_map(
      std::make_move_iterator(std::begin(entries)),
      std::make_move_iterator(std::end(entries)));
    return { std::move(temp_map) };
  }
  auto interval_map::search(std::uint32_t key) const -> std::uint32_t {
    auto it = map.upper_bound(key);
    if (it == std::begin(map)) {
      return key;
    }
    if (it != map.begin()) {
      it--;
    }
    auto& [src_range_start, dst] = *it;
    auto& [dst_range_start, range_size] = dst;
    if (key >= (src_range_start + range_size)) {
      return key;
    }
    return dst_range_start + (key - src_range_start);
  }

} // namespace aoc::day5
