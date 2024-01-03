//
// Created by andiw on 10/12/2022.
//

#ifndef AOC_DAY10_H
#define AOC_DAY10_H

#include <iostream>
#include <string>
#include <vector>
#include <array>
#ifndef NGUI
#include <raylib-cpp.hpp>
#endif

#include <AocLines.h>
#include <AocObject.h>
#include <AocDay.h>

using std::cerr, std::endl;

struct Operation {
    enum Op {
        Noop,
        Add
    } op;
    int value;
};

class Day10 : public aoc::AocObject<Day10>, public aoc::AocDay<Day10, int> {
    std::vector<Operation> m_commands;

public:
    static constexpr char FILENAME[] = "10";
    static constexpr char YEAR[] = "2022";
    static constexpr char TITLE[] = "Cathode-Ray Tube";

    static auto get_object(std::istream &stream, Day10 &day) -> bool {

        day.m_commands.clear();

        std::string line{};
        while (aoc::getline(stream, line)) {

            if (line == "noop")
                day.m_commands.push_back({Operation::Noop, 0});
            else if (line.starts_with("addx")) {
                day.m_commands.push_back({Operation::Add, std::atoi(line.c_str() + 5)});
            }
            else {
                cerr << "unknown command " << line << endl;
                return false;
            }
        }
        return true;
    }

    static int push_cycle(int& cycle) {
        if ((++cycle-20) % 40 == 0) {
            return cycle;
        }
        return 0;
    }

    int part1() const override {
        int cycle = 0;
        int score = 0;
        int reg_x = 1;
        for (Operation const& op: m_commands) {
            switch (op.op) {
                case Operation::Noop:
                    score += reg_x * push_cycle(cycle);
                    break;
                case Operation::Add:
                    score += reg_x * push_cycle(cycle);
                    score += reg_x * push_cycle(cycle);
                    reg_x += op.value;
            }
        }
        return score;
    }

    int part2() const override {
        std::array<bool, 40 * 6> screen{};
        int cycle = 0;
        int xpos = 0;
        int score = 0;
        int reg_x = 1;

        auto tick = [&screen, &cycle, &reg_x, &xpos] () {
            ++cycle;
            if (xpos >= 40) xpos = 0;
            if (xpos == reg_x || xpos == reg_x - 1 || xpos == reg_x + 1) screen[cycle] = true;
            ++xpos;
        };

        for (Operation const& op: m_commands) {
            switch (op.op) {
                case Operation::Noop:
                    tick();
                    break;
                case Operation::Add:
                    tick();
                    tick();
                    reg_x += op.value;
            }
        }

        for (int row = 0; row < 6; ++row) {
            for (int x = 0; x < 40; ++x) {
                cout << (screen[row*40+x] ? '#' : '.');
            }
            cout << endl;
        }

        return score;
    }
};
#endif //AOC_DAY10_H
