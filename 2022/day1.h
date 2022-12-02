//
// Created by andiw on 01/12/2022.
//

#ifndef AOC_DAY1_H
#define AOC_DAY1_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <ranges>

#include <aoclines.h>
#include <numeric>

using std::cerr, std::cout, std::endl;

class Calories {
    std::vector<int> m_calories;

public:
    static auto from_istream(std::istream& stream) -> std::optional<Calories> {
        std::string line{};
        Calories cals{};
        while (std::getline(stream, line)) {
            if (line.empty()) break;

            try {
                const int i = std::stoi(line);
                cals.m_calories.push_back(i);
            }
            catch(std::invalid_argument const& ex) {
                cerr << "invalid arg for int conversion: " << line << endl;
                return {};
            }
            catch(std::out_of_range const& ex) {
                cerr << "out of range for string " << line << endl;
                return {};
            }
        }
        return cals;
    }

    auto sum() const -> int {
        return std::reduce(m_calories.begin(), m_calories.end());
    }

    auto empty() const -> bool {
        return m_calories.empty();
    }
};

class Day1 {
    std::vector<Calories> m_elves;

public:
    using value = int;
    static constexpr char FILENAME[] = "1";


public:
    static auto from_istream(std::istream& stream) -> std::optional<Day1> {
        Day1 day{};

        do {
            auto elf = Calories::from_istream(stream);
            if (!elf)
                return {};
            if (elf->empty()) break;

            day.m_elves.push_back(*elf);
        } while(true);

        return day;
    }

    auto part1() const -> value {
        return std::ranges::max(
                m_elves
                | std::views::transform([](Calories const& c) { return c.sum(); }));
    }

    auto part2() -> value {
        std::vector<int> sum_of_calories;

        std::transform(m_elves.cbegin(), m_elves.cend(),
                       std::back_inserter(sum_of_calories),
                       [](Calories const& c) { return c.sum(); });

        std::sort(sum_of_calories.begin(), sum_of_calories.end(), std::greater<int>());

        return sum_of_calories[0] + sum_of_calories[1] + sum_of_calories[2];
    }

};

#endif //AOC_DAY1_H
