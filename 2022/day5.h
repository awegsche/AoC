//
// Created by andiw on 05/12/2022.
//

#ifndef AOC_DAY5_H
#define AOC_DAY5_H

#include <string>
#include <vector>
#include <iostream>
#include <re2.h>

#include <aoc_object.h>
#include <AocDay.h>

struct Command {
    int amount;
    int from;
    int to;
};

class Day5: public AocObject<Day5>, public AocDay<Day5, std::string> {
    std::vector<std::vector<char>> m_stacks;
    std::vector<Command> m_commands;


public:
    static constexpr char FILENAME[] = "5";
    static constexpr char YEAR[] = "2022";

    static auto get_object(std::istream& stream, Day5& day) -> bool {
        RE2 re_cmd{R"(move (\d+) from (\d+) to (\d+))"};

        day.m_stacks.clear();
        day.m_commands.clear();

        std::vector<std::string> stack_defs;

        std::string line{};
        while (std::getline(stream, line)) {
            if (line.empty()) break;
            stack_defs.push_back(line);
        }

        // now the last lines should be " 1  2  3  4"
        auto line_len = stack_defs[stack_defs.size()-1].size();
        int stacks = 0;
        for (int i = 1; i < line_len; i += 4) {
            day.m_stacks.emplace_back();
        }

        for (int i = stack_defs.size() - 2; i >= 0; --i) {
            auto const& def_line = stack_defs[i];
            for (int s = 1, si = 0; s < def_line.size(); s += 4) {
                char c = def_line[s];
                if (isalpha(c)) {
                    day.m_stacks[si].push_back(c);
                }
                ++si;
            }
        }

        while (std::getline(stream, line)) {
            Command cmd{};
            if (RE2::FullMatch(line, re_cmd, &cmd.amount, &cmd.from, &cmd.to)) {
                // fix 1-based index
                --cmd.to;
                --cmd.from;

                day.m_commands.push_back(cmd);
            }
        }

        return true;
    }

    auto part1() const -> value override {

        // copy the stacks (we want to have clean slate at the end of this)
        auto stacks = m_stacks;

        for (auto const& cmd: m_commands) {
            for (int i = 0; i < cmd.amount; ++i) {
                stacks[cmd.to].push_back(stacks[cmd.from].back());
                stacks[cmd.from].pop_back();
            }
        }

        std::string answer{};

        for (auto const& stack: stacks) {
            answer.push_back(stack.back());
        }

        return answer;
    }

    auto part2() const -> value override {

        // copy the stacks (we want to have clean slate at the end of this)
        auto stacks = m_stacks;

        for (auto const& cmd: m_commands) {
            auto offset = stacks[cmd.from].size() - cmd.amount;
            for (int i = 0; i < cmd.amount; ++i) {
                stacks[cmd.to].push_back(stacks[cmd.from][offset+i]);
            }
            stacks[cmd.from].resize(offset);
        }

        std::string answer{};

        for (auto const& stack: stacks) {
            answer.push_back(stack.back());
        }

        return answer;
    }
};

#endif //AOC_DAY5_H
