//
// Created by andiw on 06/12/2022.
//

#ifndef AOC_DAY6_H
#define AOC_DAY6_H

#include <iostream>
#include <string>
#include <vector>

#include <AocObject.h>
#include <AocDay.h>

using aoc::AocDay, aoc::AocObject;

constexpr int PACKET_LEN = 4;
constexpr int MESSAGE_LEN = 14;

template<int N>
auto start_of_block(std::string_view message) -> bool {
    for (int i = N - 1; i >= 1; --i) {
        for (int j = 0; j < i; ++j)
            if (message[j] == message[i]) return true;
    }
    return false;
}

class Day6 : public AocObject<Day6>, public AocDay<Day6, int> {
    std::string m_message;

public:

    static constexpr char FILENAME[] = "6";
    static constexpr char YEAR[] = "2022";
    static constexpr char TITLE[] = "Tuning Trouble";

    static auto get_object(std::istream &stream, Day6 &day) -> bool {
        return (bool) std::getline(stream, day.m_message);
    }

    int part1() const override {
        return part_n<PACKET_LEN>();
    }

    int part2() const override {
        return part_n<MESSAGE_LEN>();
    }

private:

    template<int N>
    int part_n() const {
        for (size_t i = 0; i < m_message.size() - N; ++i) {
            if (!start_of_block<N>(std::string_view{m_message.c_str() + i, N})) {
                return N + i;
            }
        }
        return -1;
    }
};

#endif //AOC_DAY6_H
