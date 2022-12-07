//
// Created by andiw on 07/12/2022.
//

#ifndef AOC_TIMING_H
#define AOC_TIMING_H

#include <chrono>
#include <iostream>
#include <iomanip>

#include "pretty.h"

using std::cout, std::cerr, std::endl, std::setw, std::setfill, std::left, std::right;

namespace aoc {
    constexpr char UNIT_COLOR[] = "\33[1m\33[38;2;128;128;128m";

    template<typename T, typename U>
    void print_time(std::chrono::duration<T, U> const &time) {
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();

        cout << setw(COL3 - 4) << std::setprecision(3) << right;

        if (nanoseconds < 1000000) {
            cout << 1.0e-3f * nanoseconds << UNIT_COLOR << " us ";
        } else if (nanoseconds < 1000000000) {
            cout << 1.0e-6f * nanoseconds << UNIT_COLOR << " ms ";
        } else {
            cout << 1.0e-9f * nanoseconds << UNIT_COLOR << "  s ";
        }
        cout << RESET;
    }

} // namespace aoc

#endif //AOC_TIMING_H
