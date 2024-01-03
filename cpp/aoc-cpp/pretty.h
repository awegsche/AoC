//
// Created by andiw on 07/12/2022.
//

#ifndef AOC_PRETTY_H
#define AOC_PRETTY_H

namespace aoc {

    constexpr char OK[] = "[ \33[38;2;50;255;50mOK\33[0m ]";
    constexpr char FAIL[] = "[\33[38;2;255;50;50mFAIL\33[0m]";
    constexpr char STAR_COLOR[] = "\33[1m\33[38;2;255;255;100m";
    constexpr char TITLE_COLOR[] = "\33[1m\33[38;2;100;185;255m";
    constexpr char RESET[] = "\33[0m";
    constexpr int COL1 = 10;
    constexpr int COL2 = 20;
    constexpr int COL3 = 10;

} // namespace aoc

#endif //AOC_PRETTY_H
