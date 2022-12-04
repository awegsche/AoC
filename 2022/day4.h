//
// Created by andiw on 04/12/2022.
//

#ifndef AOC_DAY4_H
#define AOC_DAY4_H

#include <vector>
#include <iostream>

#include "aoc_object.h"

struct Section {
    int min;
    int max;

    bool in(Section const& other) const {
        return min >= other.min && max <= other.max;
    }

    bool overlaps(Section const& other) const {
        return (min >= other.min && min <= other.max)
               || (other.min >= min && other.min <=max)
               || (max >= other.min && max <= other.max)
               || (other.max >= min && other.max <= max);
    }
};

struct Pair {
    Section section1;
    Section section2;

    bool has_containing_pair() const {
        return section1.in(section2) || section2.in(section1);
    }
    bool has_overlapping_pair() const {
        return section1.overlaps(section2);
    }
};

class Day4: public AocObject<Day4> {
    std::vector<Pair> pairs;
public:
    constexpr static char FILENAME[] = "4";
    using value = int;


    static auto get_object(std::istream& stream, Day4& day) -> bool {
        std::string line{};
        day.pairs.clear();

        while (true) {
            Pair pair{};
            if (!std::getline(stream, line, '-')) return true;
            pair.section1.min = std::stoi(line);
            if (!std::getline(stream, line, ',')) return false;
            pair.section1.max = std::stoi(line);
            if (!std::getline(stream, line, '-')) return false;
            pair.section2.min = std::stoi(line);
            if (!std::getline(stream, line)) return false;
            pair.section2.max = std::stoi(line);

            day.pairs.push_back(pair);
        }
    }

    auto part1() -> value {
        int count = 0;
        for (auto const& pair: pairs) {
            if (pair.has_containing_pair())
                count++;
        }

        return count;
    }

    auto part2() -> value {
        int count = 0;
        for (auto const& pair: pairs) {
            if (pair.has_overlapping_pair())
                count++;
        }

        return count;
    }
};
#endif //AOC_DAY4_H
