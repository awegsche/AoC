//
// Created by andiw on 02/12/2022.
//

#ifndef AOC_DAY2_H
#define AOC_DAY2_H

#include "AocLines.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <optional>
#include <ranges>
#include <numeric>
#include <algorithm>

#include <AocLines.h>
#include <AocObject.h>
#include <AocDay.h>

using aoc::AocDay, aoc::AocObject;

struct Game{
    char opponent;
    char me;
};

class Day2: public AocObject<Day2>, public AocDay<Day2, int> {
    std::vector<Game> games;

public:
    static constexpr char TITLE[] = "Rock Paper Scissors";
    static constexpr char FILENAME[] = "2";
    static constexpr char YEAR[] = "2022";

    static auto get_object(std::istream& stream, Day2& day) -> bool {
        std::string line{};

        while (aoc::getline(stream, line)) {
            day.games.push_back({line[0], line[2]});
        }
        return true;
    }

    auto part1() const -> value override {
        auto score = [] (Game const& game) {

            switch (game.me) {
                case 'X': {
                    int sc = 1;
                    switch (game.opponent) {
                        case 'A':
                            return sc + 3;
                        case 'B':
                            return sc;
                        case 'C':
                            return sc + 6;
                    }
                }
                case 'Y': {
                    int sc = 2;
                    switch (game.opponent) {
                        case 'A':
                            return sc + 6;
                        case 'B':
                            return sc + 3;
                        case 'C':
                            return sc + 0;
                    }
                }
                case 'Z': {
                    int sc = 3;
                    switch (game.opponent) {
                        case 'A':
                            return sc + 0;
                        case 'B':
                            return sc + 6;
                        case 'C':
                            return sc + 3;
                    }
                }
            }
            // panic!!!
            return -1;
        };

        int answer = 0;
        for(auto const& game: games) {
            answer += score(game);
        }

        return answer;
    }

    auto part2() const -> value override {
        auto score = [] (Game const& game) {

            switch (game.me) {
                case 'X': {
                    int sc = 0;
                    switch (game.opponent) {
                        case 'A':
                            return sc + 3;
                        case 'B':
                            return sc + 1;
                        case 'C':
                            return sc + 2;
                    }
                }
                case 'Y': {
                    int sc = 3;
                    switch (game.opponent) {
                        case 'A':
                            return sc + 1;
                        case 'B':
                            return sc + 2;
                        case 'C':
                            return sc + 3;
                    }
                }
                case 'Z': {
                    int sc = 6;
                    switch (game.opponent) {
                        case 'A':
                            return sc + 2;
                        case 'B':
                            return sc + 3;
                        case 'C':
                            return sc + 1;
                    }
                }
            }
            // panic!!!
            return -1;
        };

        int answer = 0;
        for(auto const& game: games) {
            answer += score(game);
        }

        return answer;
    }
};

#endif //AOC_DAY2_H
