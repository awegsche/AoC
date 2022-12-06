//
// Created by andiw on 01/12/2022.
//

#ifndef AOC_DAY1_H
#define AOC_DAY1_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <numeric>

#include <aoclines.h>
#include <aoc_object.h>
#include <AocDay.h>

class Day1: public AocObject<Day1>, public AocDay<Day1, int> {
    std::vector<std::vector<int>> m_elves;

public:
    static constexpr char TITLE[] = "Calorie Counting";
    static constexpr char FILENAME[] = "1";
    static constexpr char YEAR[] = "2022";

public:
    static auto get_object(std::istream& stream, Day1& day) -> bool {
        day.m_elves.clear();
        day.m_elves.emplace_back();

        std::string line{};

        while (std::getline(stream, line)) {
            if (line.empty()) {
                day.m_elves.emplace_back();
            }
            else {
                day.m_elves.back().emplace_back(std::stoi(line));
            }
        }
        return true;
    }

    auto part1() const -> value override {
        using std::views::transform;

        return std::ranges::max(
                m_elves
                | transform([](std::vector<int> const& c) { return std::reduce(c.begin(), c.end()); })
        );
    }

    auto part2() const -> value override {
        using std::views::transform;

        auto temp = m_elves | transform([](std::vector<int> const& c) { return std::reduce(c.begin(), c.end()); });
        std::vector<int> sum_of_calories{temp.begin(), temp.end()};

        std::ranges::sort(sum_of_calories.begin(), sum_of_calories.end(), std::greater<>());

        return sum_of_calories[0] + sum_of_calories[1] + sum_of_calories[2];
    }
};

#endif //AOC_DAY1_H
