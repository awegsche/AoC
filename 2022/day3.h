//
// Created by andiw on 03/12/2022.
//

#include <vector>
#include <iostream>
#include <optional>
#include <string>
#include <aoclines.h>
#include <limits>
#include <bitset>

#include <aoc_object.h>
#include <AocDay.h>


 uint8_t number_from_char(char c) {
    if (c >= 'a') return c - 'a' + 1;
    return c - 'A' + 27;
}

class Rucksack: public AocObject<Rucksack>
{
public:
    uint64_t bitmask;
    uint64_t bitmask_a;
    uint64_t bitmask_b;

public:
    static auto get_object(std::istream& stream, Rucksack& rucksack) -> bool {
        std::string line{};

        // reset values
        rucksack.bitmask = 0;
        rucksack.bitmask_a = 0;
        rucksack.bitmask_b = 0;

        if (std::getline(stream, line)) {
            for (char c: line)
               rucksack.bitmask |= (1LL << number_from_char(c));
            for (int i = 0; i < line.size()/2; ++i) {
                rucksack.bitmask_a |= (1LL << number_from_char(line[i]));
                rucksack.bitmask_b |= (1LL << number_from_char(line[i+line.size()/2]));
            }
            return true;
        }
        return false;
    }

    auto same() const -> uint8_t {
        auto mask = bitmask_a & bitmask_b;
        for (int shift = 1; shift <= 52; ++ shift)
            if ((mask >> shift) & 1)
                return shift;
    }
};

class Day3: public AocObject<Day3>, public AocDay<Day3, int> {
    std::vector<Rucksack> rucksacks;

public:
    static constexpr char FILENAME[] = "3";
    static constexpr char YEAR[] = "2022";

public:
    static auto get_object(std::istream& stream, Day3& day) -> bool {
        Rucksack rucksack;
        while(Rucksack::get_object(stream, rucksack)) {
            day.rucksacks.push_back(rucksack);
        }

        return true;
    }

    auto part1() -> value override {
        int sum = 0;
        for(auto const& rucksack: rucksacks) {
            sum += static_cast<int>(rucksack.same());
        }
        return sum;
    }

    auto part2() -> value override {
        int answer = 0;

        for (int i = 0; i < rucksacks.size()/3; ++i) {
            auto mask = rucksacks[i*3].bitmask & rucksacks[i*3+1].bitmask & rucksacks[i*3+2].bitmask;
            for (int shift = 1; shift <= 52; ++ shift)
                if ((mask >> shift) & 1)
                    answer += shift;
        }
        return answer;
    }
};